package com.Moodlight;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.os.BatteryManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.text.InputType;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.method.ScrollingMovementMethod;
import android.text.style.ForegroundColorSpan;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.GridLayout;
import android.widget.ImageButton;
import android.widget.RadioButton;
import android.widget.SeekBar;
import android.widget.Space;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.skydoves.colorpickerview.ActionMode;
import com.skydoves.colorpickerview.ColorEnvelope;
import com.skydoves.colorpickerview.ColorPickerView;
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener;

public class TerminalFragment extends Fragment implements ServiceConnection, SerialListener {

    private enum Connected { False, Pending, True }

    private String deviceAddress;
    private String newline = "\n";
    private String resolution = "1";
    private int[] seekBarValues = {63,63,63,63};
    private float[] temperatureLimits = {20,30};
    private boolean overtemperatureFlag = false;

    private TextView receiveText, textViewRed, textViewGreen, textViewBlue, textViewWhite;
    private View viewSpacer1;
    private TextView colorText;
    private TextView temperatureOnPhoneText;
    private TextView temperatureOnDeviceText;
    private ColorPickerView colorPicker;
    private ProgressDialog load_dialog;
    private RadioButton radioButtonRGB, radioButtonTemperature, radioButtonRGBFade, radioButtonRGBWStrobo, radioButtonSliders;
    private Switch switchIllumination, switchInverted;
    private CheckBox checkBoxRed, checkBoxGreen, checkBoxBlue, checkBoxWhite;
    private SeekBar seekBarRed, seekBarGreen, seekBarBlue, seekBarWhite;


    private SerialSocket socket;
    private SerialService service;
    private boolean initialStart = true;
    private Connected connected = Connected.False;
    private IntentFilter batteryIntentFilter;
    private BroadcastReceiver batteryBroadcastReceiver;
    private MenuItem menuItem;
    private boolean showTemp = true;
    private boolean visualValues[] = {true,true};

    /*
     * Lifecycle
     */
    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
        setRetainInstance(true);
        deviceAddress = getArguments().getString("device");
    }

    @Override
    public void onDestroy() {
        if (connected != Connected.False)
            disconnect();
        getActivity().stopService(new Intent(getActivity(), SerialService.class));
        super.onDestroy();
    }

    @Override
    public void onStart() {
        super.onStart();
        if(service != null)
            service.attach(this);
        else
            getActivity().startService(new Intent(getActivity(), SerialService.class)); // prevents service destroy on unbind from recreated activity caused by orientation change
    }

    @Override
    public void onStop() {
        if(service != null && !getActivity().isChangingConfigurations()) {
            service.detach();
        }
        try {
            getContext().unregisterReceiver(batteryBroadcastReceiver);
        }catch (Exception ex){

        }
        super.onStop();
    }

    @SuppressWarnings("deprecation") // onAttach(context) was added with API 23. onAttach(activity) works for all API versions
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        getActivity().bindService(new Intent(getActivity(), SerialService.class), this, Context.BIND_AUTO_CREATE);
    }

    @Override
    public void onDetach() {
        try { getActivity().unbindService(this); } catch(Exception ignored) {}
        super.onDetach();
    }

    @Override
    public void onResume() {
        super.onResume();
        if(initialStart && service !=null) {
            initialStart = false;
            getActivity().runOnUiThread(this::connect);
        }
    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder binder) {
        service = ((SerialService.SerialBinder) binder).getService();
        if(initialStart && isResumed()) {
            initialStart = false;
            getActivity().runOnUiThread(this::connect);
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        service = null;
    }

    /*
     * UI
     */
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_terminal, container, false);
        initUI(view);
        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        inflater.inflate(R.menu.menu_terminal, menu);
        menuItem = menu.findItem(R.id.status);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.status) {
            receiveText.setText("");
            return true;
        } else if (id ==R.id.resolution) {
            String[] resolutionNames = getResources().getStringArray(R.array.resolution_names);
            String[] resolutionValues = getResources().getStringArray(R.array.resolution_values);
            int pos = java.util.Arrays.asList(resolutionValues).indexOf(resolution);
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setTitle("Resolution");
            builder.setSingleChoiceItems(resolutionNames, pos, (dialog, item1) -> {
                resolution = resolutionValues[item1];
                dialog.dismiss();
                if ((switchInverted != null) & (switchIllumination != null)){
                    String res_string = String.format("%03d",Integer.parseInt(resolution));
                    if (switchIllumination.isChecked()) {
                        if (switchInverted.isChecked()){
                            send(getString(R.string.ILLUMINATION) + '\n' + getString(R.string.ON) + '\n' + getString(R.string.ON) + '\n' + res_string + '\n');
                        }
                        else{
                            send(getString(R.string.ILLUMINATION) + '\n'+getString(R.string.OFF) + '\n' + getString(R.string.ON) + '\n' + res_string + '\n');
                        }

                    } else {
                        if (switchInverted.isChecked()){
                            send(getString(R.string.ILLUMINATION) + '\n' + getString(R.string.ON) + '\n'+getString(R.string.OFF) + '\n' + res_string + '\n');
                        }
                        else{
                            send(getString(R.string.ILLUMINATION) + '\n' + getString(R.string.OFF) + '\n' + getString(R.string.OFF) + '\n' + res_string + '\n');
                        }
                    }
                }
            });
            builder.create().show();
            return true;
        } else if (id == R.id.reconnect){
            if (connected == Connected.False){
                receiveText.setText("");
                if (menuItem != null) {
                    menuItem.setIcon(getResources().getDrawable(android.R.drawable.presence_invisible));
                }
                connect();
            }
            return true;
        } else if(id == R.id.visuals){
            CharSequence[] visualNames = getResources().getStringArray(R.array.visual_names);
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setTitle("Visuals")
                    .setMultiChoiceItems(visualNames,visualValues, new DialogInterface.OnMultiChoiceClickListener() {
                        @Override
                        public void onClick(DialogInterface arg0, int arg1, boolean arg2) {
                            visualValues[arg1] = arg2;
                        }
                    })
                    .setCancelable(false)
                    .setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            showTemp = visualValues[0];
                            setTransmissionVisible(visualValues[1]);
                        }
                    })
                    .create().show();
            //showTemp
            return true;
        } else if (id == R.id.clear) {
            receiveText.setText("");
            return true;
        } else if (id == R.id.tempLimit){
            AlertDialog.Builder alert = new AlertDialog.Builder(getActivity());
            alert.setTitle("Temp Limit");
            InputMethodManager inputMethodManager = (InputMethodManager) getContext().getSystemService(Activity.INPUT_METHOD_SERVICE);
            final EditText underLimitResult = new EditText(getActivity());
            underLimitResult.setText(String.valueOf(temperatureLimits[0]));
            underLimitResult.setSingleLine();
            underLimitResult.requestFocus();
            inputMethodManager.toggleSoftInput(InputMethodManager.SHOW_FORCED,0);
            final TextView underLimit = new TextView(getActivity());
            underLimit.setText(getString(R.string.UnderLimitTemp));
            final TextView upperLimit = new TextView(getActivity());
            upperLimit.setText(getString(R.string.UpperLimitTemp));
            final EditText upperLimitResult = new EditText(getActivity());
            upperLimitResult.setText(String.valueOf(temperatureLimits[1]));
            upperLimitResult.setSingleLine();

            //use TYPE_CLASS_NUMBER for input only numbers
            underLimitResult.setRawInputType(InputType.TYPE_CLASS_NUMBER);
            upperLimitResult.setRawInputType(InputType.TYPE_CLASS_NUMBER);

            Space space1 = new Space(getActivity());
            space1.setMinimumWidth(100);
            Space space2 = new Space(getActivity());
            space2.setMinimumWidth(100);
            GridLayout lay = new GridLayout(getActivity());
            lay.setColumnCount(3);
            lay.setRowCount(2);
            lay.setOrientation(GridLayout.VERTICAL);
            lay.addView(space1);
            lay.addView(space2);
            lay.addView(underLimit);
            lay.addView(upperLimit);
            lay.addView(underLimitResult);
            lay.addView(upperLimitResult);
            alert.setView(lay);

            // Set up the buttons
            alert.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int whichButton) {
                    try {
                        temperatureLimits[0] = Float.parseFloat(underLimitResult.getText().toString());
                    } catch (NumberFormatException ex){
                        // do nothing (the temperature should stay the same when empty or an error occurs)
                    }
                    try {
                        temperatureLimits[1] = Float.parseFloat(upperLimitResult.getText().toString());
                    } catch (NumberFormatException ex){
                        // do nothing (the temperature should stay the same when empty or an error occurs)
                    }
                    if(temperatureLimits[0]>temperatureLimits[1]){
                        Toast.makeText(getActivity(), getString(R.string.Temperature1), Toast.LENGTH_LONG).show();
                        float temporarily = temperatureLimits[0];
                        temperatureLimits[0] = temperatureLimits[1];
                        temperatureLimits[1] = temporarily;
                    }
                    if (inputMethodManager.isActive() != true){
                        inputMethodManager.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);
                    }
                    if(temperatureLimits[0] != temperatureLimits[1]) {
                        int underLimit = (int) (temperatureLimits[0] * 8);
                        int upperLimit = (int) (temperatureLimits[1] * 8);
                        temperatureLimits[0] = (float) underLimit / 8;
                        temperatureLimits[1] = (float) upperLimit / 8;
                        String tempLimit = getString(R.string.TEMPLIMIT) + '\n' + String.format("%06d", underLimit) + '\n' + String.format("%06d", upperLimit);
                        send(tempLimit);
                    }
                    else {
                        Toast.makeText(getActivity(), getString(R.string.Temperature2), Toast.LENGTH_LONG).show();
                    }

                }
            });
            alert.show();
            return true;
        }else {
            return super.onOptionsItemSelected(item);
        }
    }

    /*
     * Serial + UI
     */
    private void initUI(View view){
        load_dialog =new ProgressDialog(getActivity());
        load_dialog.setMessage(getString(R.string.load_Dialog));
        load_dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        load_dialog.setIndeterminate(true);
        load_dialog.setCancelable(false);
        receiveText = view.findViewById(R.id.receive_text);                          // TextView performance decreases with number of spans
        receiveText.setTextColor(getResources().getColor(R.color.colorRecieveText)); // set as default color to reduce number of spans
        receiveText.setMovementMethod(ScrollingMovementMethod.getInstance());
        viewSpacer1 = view.findViewById(R.id.viewSpacer_1);
        temperatureOnPhoneText = view.findViewById(R.id.textTempPhone);
        temperatureOnDeviceText = view.findViewById(R.id.textTempDevice);
        batteryBroadcastReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                float temperature = (float)(intent.getIntExtra(BatteryManager.EXTRA_TEMPERATURE,0))/10;
                temperatureOnPhoneText.setText(temperature +" "+ (char) 0x00B0 +"C");
            }
        };
        batteryIntentFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);// Initialize a new IntentFilter instance
        getContext().registerReceiver(batteryBroadcastReceiver,batteryIntentFilter);// Register the broadcast receiver
        colorText = view.findViewById(R.id.textColor);
        colorPicker = view.findViewById(R.id.colorPicker);
        colorPicker.setActionMode(ActionMode.LAST);
        colorPicker.setColorListener(new ColorEnvelopeListener() {
            @Override
            public void onColorSelected(ColorEnvelope envelope, boolean fromUser) {
                colorText.setText(envelope.getHexCode().toString());
                sendColor(envelope, null, null);
            }
        });
        radioButtonRGB = view.findViewById(R.id.radioButtonColor);
        radioButtonRGB.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (((RadioButton) view).isChecked()){
                    send(getString(R.string.MODE)+'\n'+getString(R.string.MODE_RGB));
                    (new Handler()).postDelayed(this::sendColorRGB, 300);
                    colorPicker.setVisibility(View.VISIBLE);
                    setSlidersVisible(View.INVISIBLE);
                }
            }

            private void sendColorRGB() {
                sendColor(colorPicker.getColorEnvelope(), null, null);
            }
        });
        radioButtonTemperature = view.findViewById(R.id.radioButtonTemperature);
        radioButtonTemperature.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (((RadioButton) view).isChecked()){
                    send(getString(R.string.MODE)+'\n'+getString(R.string.MODE_TEMPERATURE));
                    colorPicker.setVisibility(View.INVISIBLE);
                    setSlidersVisible(View.INVISIBLE);
                }
            }
        });
        radioButtonRGBFade = view.findViewById(R.id.radioButtonRGBFade);
        radioButtonRGBFade.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (((RadioButton) view).isChecked()){
                    send(getString(R.string.MODE)+'\n'+getString(R.string.MODE_RGBFADE));
                    colorPicker.setVisibility(View.INVISIBLE);
                    setSlidersVisible(View.INVISIBLE);
                }
            }
        });
        radioButtonRGBWStrobo = view.findViewById(R.id.radioButtonRGBWStrobo);
        radioButtonRGBWStrobo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (((RadioButton) view).isChecked()){
                    send(getString(R.string.MODE)+'\n'+getString(R.string.MODE_RGBWSTROBO));
                    colorPicker.setVisibility(View.INVISIBLE);
                    setSlidersVisible(View.INVISIBLE);
                }
            }
        });
        radioButtonSliders = view.findViewById(R.id.radioButtonSlider);
        radioButtonSliders.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                send(getString(R.string.MODE)+'\n'+getString(R.string.MODE_RGB));
                (new Handler()).postDelayed(this::sendColorSlider, 300);
                colorPicker.setVisibility(View.INVISIBLE);
                setSlidersVisible(View.VISIBLE);
            }

            private void sendColorSlider() {
                sendColor(null, null, seekBarValues);
            }
        });
        switchInverted = (Switch) view.findViewById(R.id.switchInv);
        switchInverted.setVisibility(View.INVISIBLE);
        switchIllumination = (Switch) view.findViewById(R.id.switchLum);
        switchInverted.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {

                String res_string = String.format("%03d",Integer.parseInt(resolution));
                if (isChecked) {
                    if (switchIllumination.isChecked()){
                        send(getString(R.string.ILLUMINATION) + '\n' + getString(R.string.ON) + '\n' + getString(R.string.ON) + '\n' + res_string + '\n');
                    }
                    else{
                        send(getString(R.string.ILLUMINATION) + '\n' + getString(R.string.ON) + '\n' + getString(R.string.OFF) + '\n' + res_string + '\n');
                    }

                } else {
                    if (switchIllumination.isChecked()){
                        send(getString(R.string.ILLUMINATION) + '\n' + getString(R.string.OFF) + '\n' + getString(R.string.ON) + '\n' + res_string + '\n');
                    }
                    else{
                        send(getString(R.string.ILLUMINATION) + '\n' + getString(R.string.OFF) + '\n' + getString(R.string.OFF) + '\n' + res_string + '\n');
                    }
                }
            }
        });
        switchIllumination.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String res_string = String.format("%03d",Integer.parseInt(resolution));
                if (isChecked) {
                    switchInverted.setVisibility(View.VISIBLE);
                    if (switchInverted.isChecked()){
                        send(getString(R.string.ILLUMINATION) + '\n' + getString(R.string.ON) + '\n' + getString(R.string.ON) + '\n' + res_string + '\n');
                    }
                    else{
                        send(getString(R.string.ILLUMINATION) + '\n'+getString(R.string.OFF) + '\n' + getString(R.string.ON) + '\n' + res_string + '\n');
                    }

                } else {
                    switchInverted.setVisibility(View.INVISIBLE);
                    if (switchInverted.isChecked()){
                        send(getString(R.string.ILLUMINATION) + '\n' + getString(R.string.ON) + '\n'+getString(R.string.OFF) + '\n' + res_string + '\n');
                    }
                    else{
                        send(getString(R.string.ILLUMINATION) + '\n' + getString(R.string.OFF) + '\n' + getString(R.string.OFF) + '\n' + res_string + '\n');
                    }
                }
            }
        });
        textViewRed = view.findViewById(R.id.textViewRed);
        textViewGreen = view.findViewById(R.id.textViewGreen);
        textViewBlue = view.findViewById(R.id.textViewBlue);
        textViewWhite = view.findViewById(R.id.textViewWhite);
        seekBarRed = view.findViewById(R.id.seekBarRed);
        seekBarRed.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                // void (must be Impelemented)
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                // void (must be Impelemented)
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                seekBarValues[0] = seekBar.getProgress();
                sendColor(null, null, seekBarValues);
            }
        });
        seekBarGreen = view.findViewById(R.id.seekBarGreen);
        seekBarGreen.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                // void (must be Impelemented)
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                // void (must be Impelemented)
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                seekBarValues[1] = seekBar.getProgress();
                sendColor(null, null, seekBarValues);
            }
        });
        seekBarBlue = view.findViewById(R.id.seekBarBlue);
        seekBarBlue.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                // void (must be Impelemented)
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                // void (must be Impelemented)
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                seekBarValues[2] = seekBar.getProgress();
                sendColor(null, null, seekBarValues);
            }
        });
        seekBarWhite = view.findViewById(R.id.seekBarWhite);
        seekBarWhite.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                // void (must be Impelemented)
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                // void (must be Impelemented)
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                seekBarValues[3] = seekBar.getProgress();
                sendColor(null, null, seekBarValues);
            }
        });
        checkBoxRed = view.findViewById(R.id.checkBoxRed);
        checkBoxRed.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    seekBarValues[0] = seekBarRed.getProgress();
                    seekBarRed.setEnabled(true);
                }
                else{
                    seekBarValues[0] = 0;
                    seekBarRed.setEnabled(false);
                }
                sendColor(null, null, seekBarValues);
            }
        });
        checkBoxGreen = view.findViewById(R.id.checkBoxGreen);
        checkBoxGreen.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    seekBarValues[1] = seekBarGreen.getProgress();
                    seekBarGreen.setEnabled(true);
                }
                else{
                    seekBarValues[1] = 0;
                    seekBarGreen.setEnabled(false);
                }
                sendColor(null, null, seekBarValues);
            }
        });
        checkBoxBlue = view.findViewById(R.id.checkBoxBlue);
        checkBoxBlue.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    seekBarValues[2] = seekBarBlue.getProgress();
                    seekBarBlue.setEnabled(true);
                }
                else{
                    seekBarValues[2] = 0;
                    seekBarBlue.setEnabled(false);
                }
                sendColor(null, null, seekBarValues);
            }
        });
        checkBoxWhite = view.findViewById(R.id.checkBoxWhite);
        checkBoxWhite.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    seekBarValues[3] = seekBarWhite.getProgress();
                    seekBarWhite.setEnabled(true);
                }
                else{
                    seekBarValues[3] = 0;
                    seekBarWhite.setEnabled(false);
                }
                sendColor(null, null, seekBarValues);
            }
        });
    }

    private void setTransmissionVisible(boolean status){
        if(status){
            receiveText.setVisibility(View.VISIBLE);
            viewSpacer1.setVisibility(View.VISIBLE);
        }else {
            receiveText.setVisibility(View.INVISIBLE);
            viewSpacer1.setVisibility(View.INVISIBLE);
        }
    }

    private void setSlidersVisible(int visible){
        textViewRed.setVisibility(visible);
        textViewGreen.setVisibility(visible);
        textViewBlue.setVisibility(visible);
        textViewWhite.setVisibility(visible);
        seekBarRed.setVisibility(visible);
        seekBarGreen.setVisibility(visible);
        seekBarBlue.setVisibility(visible);
        seekBarWhite.setVisibility(visible);
        checkBoxRed.setVisibility(visible);
        checkBoxGreen.setVisibility(visible);
        checkBoxBlue.setVisibility(visible);
        checkBoxWhite.setVisibility(visible);
    }

    private void connect() {
        try {
            load_dialog.show();
            BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            BluetoothDevice device = bluetoothAdapter.getRemoteDevice(deviceAddress);
            String deviceName = device.getName() != null ? device.getName() : device.getAddress();
            status("connecting...");
            connected = Connected.Pending;
            socket = new SerialSocket();
            service.connect(this, "Connected to " + deviceName);
            socket.connect(getContext(), service, device);
        } catch (Exception e) {
            onSerialConnectError(e);
        }
    }

    private void disconnect() {
        connected = Connected.False;
        service.disconnect();
        socket.disconnect();
        socket = null;
    }

    private void send(String str) {
        if(connected != Connected.True) {
            Toast.makeText(getActivity(), "not connected", Toast.LENGTH_SHORT).show();
            return;
        }
        if (overtemperatureFlag){
            Toast.makeText(getActivity(), getString(R.string.OvertemperatureToast), Toast.LENGTH_LONG).show();
            return;
        }
        try {
            SpannableStringBuilder spn = new SpannableStringBuilder(str + '\n');
            spn.setSpan(new ForegroundColorSpan(getResources().getColor(R.color.colorSendText)), 0, spn.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
            receiveText.append(spn);
            byte[] data = (str + newline).getBytes();
            socket.write(data);
        } catch (Exception e) {
            onSerialIoError(e);
        }
    }

    private void receive(byte[] data) {
        String receivedData = new String(data);

        if ((data.length == 16)&&(data[0] == 'T')) {
            if (showTemp) {
                receiveText.append(receivedData);
            }
            try {
                int offset = 65;
                short temp_s = (short) (((short) (data[11] - offset) << 12) | ((short) (data[12] - offset) << 8) | ((short) (data[13] - offset) << 4) | ((short) (data[14] - offset)));
                int temp = (int) temp_s;
                float temperature = (float) temp / 8;
                String value = String.format("%.1f", temperature);
                temperatureOnDeviceText.setText(value + " " + getString(R.string.celsius));
            } catch (Exception ex) {
                Toast.makeText(getContext(), ex.getClass().getSimpleName() + ex.getMessage(), Toast.LENGTH_LONG).show();
            }
        } else if ((data.length == 16)&&(data[0] == 'O')) {
            if (data[14] == '1'){
                AlertDialog alertdialog = new AlertDialog.Builder(getContext())
                        .setTitle(getString(R.string.OvertemperatureTitle))
                        .setMessage(getString(R.string.OvertemperatureMessage))
                        .setPositiveButton(android.R.string.ok, null)
                        .setIcon(android.R.drawable.ic_dialog_alert)
                        .show();
                overtemperatureFlag = true;
                colorPicker.setVisibility(View.VISIBLE);
                setSlidersVisible(View.INVISIBLE);
                radioButtonRGB.setChecked(true);
                radioButtonSliders.setChecked(false);
                radioButtonTemperature.setChecked(false);
                radioButtonRGBFade.setChecked(false);
                radioButtonRGBWStrobo.setChecked(false);
            } else {
                overtemperatureFlag = false;
            }
        } else{
            receiveText.append(receivedData);
        }
    }

    private void status(String str) {
        SpannableStringBuilder spn = new SpannableStringBuilder('\n'+str);
        spn.setSpan(new ForegroundColorSpan(getResources().getColor(R.color.colorStatusText)), 0, spn.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        receiveText.append(spn);
    }


    SensorEventListener sensorEventListener = new SensorEventListener(){
        public void onAccuracyChanged(Sensor sensor, int accuracy) {}
        public void onSensorChanged(SensorEvent event) {
            float[] values = event.values;
            if (temperatureOnPhoneText != null){
                temperatureOnPhoneText.setText(values[0] + getString(R.string.celsius));
            }
        }
    };

    /*
     * SerialListener
     */
    @Override
    public void onSerialConnect() {
        status("connected");
        connected = Connected.True;
        setModeAndData();
        load_dialog.dismiss();
        if (menuItem != null){
            menuItem.setIcon(getResources().getDrawable(android.R.drawable.presence_online));
        }
    }

    @Override
    public void onSerialConnectError(Exception e) {
        status("connection failed: " + e.getMessage());
        disconnect();
        load_dialog.dismiss();
        if (menuItem != null) {
            menuItem.setIcon(getResources().getDrawable(android.R.drawable.presence_busy));
        }
    }

    @Override
    public void onSerialRead(byte[] data) {
        receive(data);
    }

    @Override
    public void onSerialIoError(Exception e) {
        status("connection lost: " + e.getMessage());
        disconnect();
        getFragmentManager().beginTransaction().replace(R.id.fragment, new DevicesFragment()).addToBackStack(null).commit();
    }

    void sendColor(ColorEnvelope colorEnvelope, int[] colorArray, int[] sliderArray){
        try {
            int[] argbArray, rgbwArray={0,0,0,0};
            if (colorEnvelope == null){
                argbArray = colorArray;
            }else {
                argbArray = colorEnvelope.getArgb();
            }
            if (argbArray != null) {
                rgbwArray[3] = Math.min(argbArray[1], Math.min(argbArray[2], argbArray[3]));    // White
                rgbwArray[0] = argbArray[1] - rgbwArray[3];                                     // Red
                rgbwArray[1] = argbArray[2] - rgbwArray[3];                                     // Green
                rgbwArray[2] = argbArray[3] - rgbwArray[3];                                     // Blue
            } else {
                rgbwArray = sliderArray;
            }
            if ((connected == Connected.True) & ((radioButtonRGB.isChecked() == true)|(radioButtonSliders.isChecked() == true))){
                String colorValue;
                for (int i = 0; i<4; i++){
                    colorValue = String.format("%03d",rgbwArray[i]);
                    send(colorValue);

                }
            }
        }catch (Exception ex){
            Toast.makeText(getContext(),getString(R.string.Exception_Color) + '\n' + ex.getClass().getSimpleName() +'\n' + ex.getMessage(),Toast.LENGTH_LONG).show();
        }

    }
    void sendColorStartup(){
        int[] colArr = {0,255,255,255};
        sendColor(null,colArr, null);
    }

    void setModeAndData(){
        send(getString(R.string.MODE)+'\n'+getString(R.string.MODE_RGB));
        (new Handler()).postDelayed(this::sendColorStartup, 300);
    }

    void setColorPicker(int[] rgbwArray){
        if (rgbwArray.length == 4) {
            float[] rgbArray = {0, 0, 0};
            // convert to RGB
            rgbArray[0] = rgbwArray[0] + rgbwArray[3];    // red
            rgbArray[1] = rgbwArray[1] + rgbwArray[3];    // green
            rgbArray[2] = rgbwArray[2] + rgbwArray[3];    // blue


            rgbArray[0] = rgbArray[0] / 255;      // red [0 ... 1]
            rgbArray[1] = rgbArray[1] / 255;      // green [0 ... 1]
            rgbArray[2] = rgbArray[2] / 255;      // blue [0 ... 1]

            float C_max = Math.max(rgbArray[0], Math.max(rgbArray[1], rgbArray[2]));
            float C_min = Math.min(rgbArray[0], Math.min(rgbArray[1], rgbArray[2]));

            float delta = C_max - C_min;
            float H = 0;
            float S;
            if (delta == 0) {
                H = 0;
            } else if (C_max == rgbArray[0]) {
                H = 60 * ((rgbArray[1] - rgbArray[2]) / delta) * 2;
            } else if (C_max == rgbArray[1]) {
                H = (60 * (((rgbArray[2] - rgbArray[0]) / delta) + 2)) + 50;
            } else if (C_max == rgbArray[2]) {
                H = (60 * (((rgbArray[0] - rgbArray[1]) / delta) + 4)) + 30;
            }
            if (C_max == 0) {
                S = 0;
            } else {
                S = delta / C_max;
            }
            S = 450 * S;
            float rad = (float) Math.toRadians(H);
            float x = (float) ((S * Math.cos(rad)));
            float y = (float) ((S * Math.sin(rad)));

            x = Math.round(x + 450);
            y = Math.round(Math.abs(y - 450));
            colorPicker.setSelectorPoint((int) x, (int) y);
        }
    }
}
