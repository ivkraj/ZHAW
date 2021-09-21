% Mode S Sekundärradar DF11 (all-call) downlink message Empfänger mit auf 1090 MHz aufgezeichneten I/Q Daten as input
% Ohne CRC check, weil dieser mit unbekannter Interrogator ID maksiert ist.
%
% ZHAW, ISC, sarp, 27.03.2020

clear all;
close all;

load('IQ_Data_DF11.mat');                   % Load I/Q Data at fs = 2.4 MSamples/s (FrontEndSampleRate), duration ca. 0.7 s, received at 1090 MHz

% Set initial parameters
duration           = 0.700;                 % RX Duration (s)
FrontEndSampleRate = 2.4E6;                 % Samples per second of raw I/Q data, ranges 225–300 kHz 900–2560 kHz
FrameLength        = 375000;                % Number of I/Q samples per block
InterpolationFactor = 5;                    % Upsampling by factor InterpolationFactor. This gives a sampling rate of 5*2.4E6 = 12 Ms/s (12 Samples per symbol)
Preamble = 2*[1 0 1 0 0 0 0 1 0 1 0 0 0 0 0 0]-1; % Mode S preamble, duration 8 us, zero mean, one entry = 1 chip of duration Tchip = 0.5 us
Tchip = 0.5E-6;                             % Duration of half a symbol
Preamble_data = [1; 1; 0; 0];               % PPM data contained in 4 pulses of preamble
Num_samples_up_preamble = round(FrontEndSampleRate*InterpolationFactor*length(Preamble)*Tchip); % Number of samples in preamble per chip after interpolation
MF_PPM_data = [1 -1];                       % Matched Filter for 1 PPM Symbol
Num_samples_up_symbol = round(FrontEndSampleRate*InterpolationFactor*length(MF_PPM_data)*Tchip); % Number of samples per symbol after interpolation
Th_preamble_detection = 3;                  % Threshold for preamble detection, to be optimized manually
delta_samples_preamble = -[84 72 48 36];    % Offset in number of samples between correlation peak of preamble and preamble sequence samples after matched filter 
delta_samples_DF_field = [12 24 36 48 60];  % Offset in number of samples between correlation peak of preamble and downlink format (DF) field samples after matched filter 
delta_samples_AA_field = [108 120 132 ...   % Offset in number of samples between correlation peak of preamble and address announced (AA) field samples after matched filter 
    144 156 168 180 192 204 216 ...
    228 240 252 264 276 288 300 ...
    312 324 336 348 360 372 384];
delta_samples_plot_range = [-130:800];		% Range of samples around detected preamble correlation peak for plotting

% Create upsampled preamble and matched filter signals
Preamble_up = kron(Preamble, ones(1, Num_samples_up_preamble/length(Preamble)));         % Copy each sample (Num_samples_up_preamble/length(Preamble)) times for interpolated preamble
MF_PPM_data_up = kron(MF_PPM_data, ones(1, Num_samples_up_symbol/length(MF_PPM_data)));  % Copy each sample (Num_samples_up_symbol/length(MF_PPM_data)) times for interpolated matched filter

% Initialize variables
address_announced_found = [];       % Address announced (ICAO aircraft address) found in DF=11 messages
radio_time = 0;                     % RX Time

fprintf('\n Receiving and decoding Address Announced (AA) in Mode S DF11 Messages (ICAO Aircraft Address)...\n')

for frame_number = 0 : (floor(duration * FrontEndSampleRate / FrameLength)-1)

    % Initialize variables
    indices_preamble_raw = [];  % Sampling indices of preamble correlation peaks exceeding threshold
    indices_preamble = [];      % Sampling indices of preamble correlation peaks exceeding threshold and having local maximum
    preamble_sequence = [];     % Temp. preamble data sequence
    preamble_index_found = [];  % Found sampling indices of indices_preamble which have correct preamble data sequence 

    % Get I/Q data block
    data_raw = IQ_Data_Baseband([1:FrameLength]+frame_number*FrameLength);  % Get new block of I/Q data from loaded data in IQ_Data_Baseband

    % Upsample using interpolation, includes anti-imaging filter
    data_up = interp(data_raw,InterpolationFactor);

    % Detect envelope (amplitude) for later PPM demodulation, including preamble
    data_up_abs = abs(data_up);

    % Search preambles using correlations exceeding a threshold, C = fftfilt(A, B) produces
    % max{length(A), length(B)} length vector C of cross-correlation values, where A is the known preamble and B contains the received signal.
    corr_preamble = fftfilt(fliplr(Preamble_up), data_up_abs-movmean(data_up_abs, [Num_samples_up_preamble-1 0])); % Cross-corralation, remove sliding trailing window mean
    indices_preamble_raw = find(corr_preamble > Th_preamble_detection).';   % Find indices which exceed a threshold

    % Keep only maximas of correlation by checking neighbouring samples on either side,
    % both must have smaller magnitude
    for index_rx = indices_preamble_raw
        if ((index_rx > -delta_samples_plot_range(1)) && (index_rx < length(corr_preamble))) % Make sure enough samples available
            if((corr_preamble(index_rx) > corr_preamble(index_rx-1)) && (corr_preamble(index_rx) > corr_preamble(index_rx+1)))
                indices_preamble = [indices_preamble index_rx];   % Add found peak 
            end % if
        end % if
    end % for


    % Apply matched Filter and binary decision for PPM data detection, including preamble
    MF_out_soft = fftfilt(fliplr(MF_PPM_data_up), data_up_abs);                % Matched filtering of RX data
    MF_out_hard = uint8((sign(MF_out_soft)+1)/2);                              % Binary decision and conversion BPSK -> NRZ


    % For all detected preambles (peaks) exceeding a threshold
    for index_rx = indices_preamble % Try all correlation maxima

        if ((index_rx + delta_samples_preamble(1) > 0) && (length(MF_out_hard) >= index_rx + delta_samples_AA_field(end)))           % Make sure enough samples available 
            preamble_sequence = MF_out_hard(index_rx+delta_samples_preamble); % Get MF output samples corresponding to 4 PPM Symbols (pulses) in preamble
        end % if

        if(isequal(preamble_sequence, Preamble_data))% Check if correct preamble sequence present
            preamble_index_found = [preamble_index_found index_rx]; % Add found preamble

            % Check downlink format field contains DF = 11 (all call reply)
            df = bi2de(MF_out_hard(index_rx+delta_samples_DF_field).', 'left-msb');

            if df == 11
                % Decode address announced (AA), this is the fixed 24 bit ICAO aircraft address, also contained in ADS B Messages DF = 17 and 18
                address_announced = dec2hex(bi2de(MF_out_hard(index_rx+delta_samples_AA_field).', 'left-msb'));
                fprintf('%s%s%s%f\n', 'AA: ', address_announced, ' time [s]: ', radio_time+index_rx/(FrontEndSampleRate*InterpolationFactor));

                % Plot absolute value of raw I/Q data
                subplot(3,1,1)
                y_data_up_abs = data_up_abs(index_rx+delta_samples_plot_range);	% Get samples around found peak, covering preamble and data block of 56 us with some reserve 
                t_data_up_abs  = [0:length(y_data_up_abs)-1]/(FrontEndSampleRate*InterpolationFactor)*1E6;	% Generate time axis values
                plot(t_data_up_abs, y_data_up_abs); 
                title('Absolute value of raw I/Q data (envelope)');
                xlabel('time [us]');
                ylabel('Envelope');

                % Plot preamble correlation
                subplot(3,1,2)
                y_corr = corr_preamble(index_rx+delta_samples_plot_range);	% Get samples around found peak, covering preamble and data block of 56 us with some reserve 
                t_corr = [0:length(y_corr)-1]/(FrontEndSampleRate*InterpolationFactor)*1E6;	% Generate time axis values
                plot(t_corr, y_corr); 
                hold on;
                plot(t_corr(-delta_samples_plot_range(1)+1), y_corr(-delta_samples_plot_range(1)+1), 'ro');
                title('Preamble Correlation');
                xlabel('time [us]');
                ylabel('Correlation Magnitude');
                legend('Correlation Magnitude','Detected preamble peak');

                % Plot sampling points for data decoding at matched filter output
                subplot(3,1,3)
                y_MF_out_soft = MF_out_soft(index_rx+delta_samples_plot_range);
                t_MF = [0:length(y_MF_out_soft)-1]/(FrontEndSampleRate*InterpolationFactor)*1E6;	% Generate time axis values
                plot(t_MF, y_MF_out_soft);
                hold on;
                title('Matched Filter soft output');
                xlabel('time [us]');
                ylabel('MF soft output');
                plot(t_MF(-delta_samples_plot_range(1)+1+delta_samples_preamble), y_MF_out_soft(-delta_samples_plot_range(1)+1+delta_samples_preamble), 'ro');
                plot(t_MF(-delta_samples_plot_range(1)+1+delta_samples_DF_field), y_MF_out_soft(-delta_samples_plot_range(1)+1+delta_samples_DF_field), 'go');
                plot(t_MF(-delta_samples_plot_range(1)+1+delta_samples_AA_field), y_MF_out_soft(-delta_samples_plot_range(1)+1+delta_samples_AA_field), 'ko');
                legend('MF soft output', 'Sampling points preamble data', 'Sampling points DF field', 'Sampling points AA field');

                pause; 	% Wait for key pressed

                subplot(3,1,1); hold off;
                subplot(3,1,2); hold off; 
                subplot(3,1,3); hold off; 
            end % if


        end % if
    end % for
  
    radio_time = radio_time + length(data_raw)/FrontEndSampleRate;          % update rx time
 
end % for  
     	


