% Wideband FM Receiver (Mono) with I/Q Data as input
%
% ZHAW, ISC, sarp, 19.03.2020, 15.04.2021: Plot 1 angepasst
% Audio Probleme: Bei vorzeitigem Programmabbruch kann manuelles ausführen
% von release(deviceWriter) im Command Window nötig sein.

clear all;
close all;

load('IQ_Data_SRF1');                       % Load I/Q Data at fs = 1 MSamples/s (FrontEndSampleRate), duration 12s

% Set initial parameters
duration           = 12;                    % RX Duration (s)
FrontEndSampleRate = 1000e3;                % Samples per second of raw I/Q data, ranges 225–300 kHz 900–2560 kHz
fc                 = 94600000;              % Center frequency (Hz), integer, SRF1 Uetliberg
f_offset_IQ        = -49890;                % RX Frequency offset to avoid DC offset in IQ Data, Configured value = -50000 Hz, exact value -49890 Hz 
FrameLength        = 375000;                % Number of I/Q samples per block
SampleRate_Audio   = 40e3;                  % Samples per second for audio processing
Channel_BW 	       = 180000;                % RF Channel BW (Hz)
Audio_BW           = 15000;                 % Audio BW mono signal (Hz)
n_decimate         = round(FrontEndSampleRate/SampleRate_Audio);	% Decimation factor RF -> Audio
f0_deemph          = 3183;                                          % Cutoff frequency (Hz) of deemphasis lowpass filter (tau = 50 us) 

% Create lowpass filtercoefficients
[b_filt_channel, a_filt_channel] = butter(12, (Channel_BW/2)/(FrontEndSampleRate/2));       % 180 kHz BW lowpass channel filter (+/- 90 kHz), order = 12
[b_filt_audio, a_filt_audio] = butter(12, Audio_BW/(FrontEndSampleRate/2));                 % 15 kHz BW lowpass audio (mono) filter, order = 12
[b_filt_deemph,a_filt_deemph] = butter(1, f0_deemph/(SampleRate_Audio/2));                  % 3183 Hz BW deemphasis lowpass filter, order = 1



% Create spectrum analyzer system objects  

hSpectrum_iq = dsp.SpectrumAnalyzer(...
    'Name',             'PSD I/Q Signal mit Max. Hold',...
    'Title',            'PSD I/Q Signal mit Max. Hold', ...
    'Method',           'Welch', ...
    'SpectrumType',     'Power density', ...
    'FrequencySpan',    'Full', ...
    'SampleRate',       FrontEndSampleRate, ...
    'SpectralAverages', 50, ...
    'FrequencyOffset',  fc, ...
    'YLimits',          [-100,10], ...
    'YLabel',           'Magnitude-squared, dB', ...
    'PlotMaxHoldTrace', true, ...
    'Position',         figposition([50 30 30 40]));

hSpectrum_Audio = dsp.SpectrumAnalyzer(...
    'Name',             'PSD Audiosignal',...
    'Title',            'PSD Audiosignal', ...
    'Method',           'Welch', ...
    'SpectrumType',     'Power density', ...
    'FrequencySpan',    'Full', ...
    'SampleRate',       SampleRate_Audio, ...
    'SpectralAverages', 50, ...
    'FrequencyOffset',  0, ...
    'YLimits',          [-100,10], ...
    'YLabel',           'Magnitude-squared, dB', ...
    'PlotMaxHoldTrace', false, ...
    'Position',         figposition([60 40 30 40]));

% Initialise audio streaming, default audio device is used
deviceWriter = audioDeviceWriter('SampleRate', SampleRate_Audio, 'SupportVariableSizeInput', true);   % Audio streaming auf default audio device
setup(deviceWriter, 0);
  
% Block-wise stream processing (as would be done when receiving I/Q data
% directly from an RTL SDR Stick

for frame_number = 1 : ceil(duration * FrontEndSampleRate / FrameLength)-1
    % Get I/Q data block
    data_raw = IQ_Data_Baseband([1:FrameLength]+frame_number*FrameLength);  % Get new block of I/Q data from loaded data in IQ_Data_Baseband
    
    % Shift frequency by f_offset_IQ
    data_raw = data_raw .* exp(1i*2*pi*f_offset_IQ*1/FrontEndSampleRate*[0:length(data_raw)-1]); 
    step(hSpectrum_iq, data_raw.');                                 % Plot PSD Baseband I/Q data spectrum

    % LP Filter I/Q Data to select channel of 180 kHz BW
    y_fm_bb = filter(b_filt_channel,a_filt_channel,data_raw); 
    y_fm_bb = y_fm_bb(length(b_filt_channel):end);                        % Remove first length(b_filt_channel)samples to avoid clicking noise at beginning of filtered output

    % FM demodulation via I/Q delta theta detection, y_audio_wide
    % includes pilot, stereo and RDS signals
    %w_fm_bb = angle(y_fm_bb(2:end).*conj(y_fm_bb(1:(end-1))));
    
    for k=2:length(y_fm_bb)
        w_fm_bb(k) = angle(y_fm_bb(k).*conj(y_fm_bb(k-1)));
    end
    
    %w_fm_bb = zeros(1, length(y_fm_bb)-1); % BITTE MIT FM DEMODULATION ÜBERSCHREIBEN
    w_fm_bb = w_fm_bb - mean(w_fm_bb);                              % Remove DC component (due to possible frequency offset)

    % LP Filter 15 kHz wide Audio Signal (Mono) and downsampling
    w_fm_bb_15kHz = filter(b_filt_audio,a_filt_audio, w_fm_bb);
    w_fm_bb_15kHz = w_fm_bb_15kHz(length(b_filt_audio):end);        % Remove first length(b_filt_audio)samples to avoid clicking noise at beginning of filtered output
    y_audio = downsample(w_fm_bb_15kHz.', n_decimate);              % Decimate signal

    % Audio deemphasis (LP Filter 3183 Hz wide) and audio output
    y_audio_deemph = filter(b_filt_deemph, a_filt_deemph, y_audio);  % Deemphasis lowpass filtering
    deviceWriter((y_audio_deemph));                                  % Audio streaming output
    step(hSpectrum_Audio, y_audio_deemph);                           % Plot PSD of audio spectrum

end
	
% Plot I/Q Signal
plot(real(y_fm_bb), 'g-'); 
hold on;
plot(imag(y_fm_bb), 'r-'); 
title('I/Q Signal im Zeitbereich (letztes Frame)');
xlabel('Sample Index');
ylabel('y_{FM\_BB}');
legend('Realanteil', 'Imaginäranteil');

% Release all System objects
release(hSpectrum_iq);
release(hSpectrum_Audio);
release(deviceWriter);

%%
NFFT = length(y_fm_bb);
fs= FrontEndSampleRate;
f = [0:NFFT-1]*fs/NFFT;
Y = 20*log10(abs(fft(y_fm_bb)/NFFT));
figure();
plot(f,Y)

