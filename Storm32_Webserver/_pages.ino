

void sendWebpage(){
  if (page == WEBPAGE_MAIN){
    crc_initialize();
    String wp = webPageMain;
    if (errorMsg != "") wp = wp + "<br>DEBUG: " + errorMsg;
    errorMsg = "";
    wp += interpretReply();
    server.send(200, "text/html", wp);
  }

  
  else if (page == WEBPAGE_GET_PARAMS){
    readReply('g');
    loadParamsFromReply();
      errorMsg = String(activeParams[0]);
    String wpExtra = "<br>bytes=" + String(replyLength) + "  DEBUG = " + errorMsg + "<br>";
    for (int i = 0; i < replyLength; i++){
      if (i % 16 == 0){
        wpExtra = wpExtra + " _#" + String(i);
      }
      wpExtra = wpExtra + "[" + String(byte(reply[i]))+ "] ";
    }
    
      //VERIFIED
      for (byte i = 0; i < 3; i++){
        wpExtra = wpExtra + "<br>" + String(getPID(i, 0));
        wpExtra += "," + String(getPID(i, 1), 1);
        wpExtra += "," + String(getPID(i, 2), 4);
        wpExtra += "," + String(int(getPID(i, 3)));
      }
      //VERIFIED
      wpExtra = wpExtra + "<br>gyroLPF=" + String(getActiveParam(PARAM_GYRO_LPF));
      
      //VERIFIED
      wpExtra = wpExtra + "<br>pitchMotorPoles=" + String(getActiveParam(PARAM_PITCH_MOTOR_POLES));
      wpExtra = wpExtra + "<br>pitchMotorDir=" + String(getActiveParam(PARAM_PITCH_MOTOR_DIR));
      wpExtra = wpExtra + "<br>pitchStartupMotorDir=" + String(getActiveParam(PARAM_PITCH_MOTOR_STARTUP_DIR));
      wpExtra = wpExtra + "<br>pitchStartupMotorPos=" + String(getActiveParam(PARAM_PITCH_MOTOR_STARTUP_POS));
      
      //VERIFIED
      wpExtra = wpExtra + "<br>rollMotorPoles=" + String(getActiveParam(PARAM_ROLL_MOTOR_POLES));
      wpExtra = wpExtra + "<br>rollMotorDir=" + String(getActiveParam(PARAM_ROLL_MOTOR_DIR));
      wpExtra = wpExtra + "<br>rollStartupMotorPos=" + String(getActiveParam(PARAM_ROLL_MOTOR_STARTUP_POS));
      
      //VERIFIED
      wpExtra = wpExtra + "<br>yawMotorPoles=" + String(getActiveParam(PARAM_YAW_MOTOR_POLES));
      wpExtra = wpExtra + "<br>yawMotorDir=" + String(getActiveParam(PARAM_YAW_MOTOR_DIR));
      wpExtra = wpExtra + "<br>yawStartupMotorPos=" + String(getActiveParam(PARAM_YAW_MOTOR_STARTUP_POS));
      
      //VERIFIED
      wpExtra = wpExtra + "<br>UARTX=" + String(getActiveParam(PARAM_UARTX)); //adr = 48  //add 23
      wpExtra = wpExtra + "<br>LIPO cells=" + String(getActiveParam(PARAM_LIPO_CELLS)); //adr = 49
      wpExtra = wpExtra + "<br>LIPO v/cell=" + String(getActiveParam(PARAM_LIPO_VOLTAGE)); //adr = 50
      wpExtra = wpExtra + "<br>IMU orientation=" + String(getActiveParam(PARAM_IMU_ORIENTATION)); //adr = 51
      wpExtra = wpExtra + "<br>IMU Mapping=" + String(getActiveParam(PARAM_IMU_MAPPING)); //adr = 52
      wpExtra = wpExtra + "<br>IMU2 configuration=" + String(getActiveParam(PARAM_IMU2_CONFIG)); //adr = 53
      wpExtra = wpExtra + "<br>IMU2 orientation=" + String(getActiveParam(PARAM_IMU2_ORIENTATION)); //adr = 54
      
      //VERIFIED
      wpExtra = wpExtra + "<br>Pitch Motor Usage=" + String(getActiveParam(PARAM_PITCH_MOTOR_USAGE)); //adr = 55
      wpExtra = wpExtra + "<br>Roll Motor Usage=" + String(getActiveParam(PARAM_ROLL_MOTOR_USAGE)); //adr = 56
      wpExtra = wpExtra + "<br>Yaw Motor Usage=" + String(getActiveParam(PARAM_YAW_MOTOR_USAGE)); //adr = 57
      wpExtra = wpExtra + "<br>Pitch Offset=" + String(getActiveParam(PARAM_PITCH_OFFSET)); //adr = 58
      wpExtra = wpExtra + "<br>Roll Offset=" + String(getActiveParam(PARAM_ROLL_OFFSET)); //adr = 59
      wpExtra = wpExtra + "<br>Yaw Offset=" + String(getActiveParam(PARAM_YAW_OFFSET)); //adr = 60
      
      wpExtra = wpExtra + "<br>? IMU AHRS=" + String(getActiveParam(PARAM_IMU_AHRS)); //adr = 61
      wpExtra = wpExtra + "<br>? ACC Compensation Method=" + String(getActiveParam(PARAM_ACC_COMP_METHOD)); //adr = 65
      wpExtra = wpExtra + "<br>? MAVLINK COM port=" + String(getActiveParam(PARAM_MAVLINK_COM_PORT)); //adr = 66
      wpExtra = wpExtra + "<br>? UART Baud Rate=" + String(getActiveParam(PARAM_UART_BAUD)); //adr = 68 //'default', '9600', '19200', '38400', '57600', '115200', '230400', '460800'
      wpExtra = wpExtra + "<br>? UART2 Baud Rate=" + String(getActiveParam(PARAM_UART2_BAUD)); //adr = 69 //'default', '9600', '19200', '38400', '57600', '115200', '230400', '460800'
      wpExtra = wpExtra + "<br>? USB Baud Rate=" + String(getActiveParam(PARAM_USB_BAUD)); //adr = 70 //'default', '9600', '19200', '38400', '57600', '115200', '230400', '460800'
      wpExtra = wpExtra + "<br>? ACC LPF=" + String(getActiveParam(PARAM_ACC_LPF)); //adr = 71 
      wpExtra = wpExtra + "<br>? IMU2 FeedForward LPF=" + String(getActiveParam(PARAM_IMU2_FEEDFORWARD_LPF)); //adr = 72  //Add 7
      wpExtra = wpExtra + "<br>? Roll Yaw PD Mixing=" + String(getActiveParam(PARAM_ROLL_YAW_PD_MIXING)); //adr = 73
      wpExtra = wpExtra + "<br>? Low Voltage Limit=" + String(getActiveParam(PARAM_LOW_VOLTAGE_LIMIT)); //adr = 74
      wpExtra = wpExtra + "<br>? Voltage Correction=" + String(getActiveParam(PARAM_VOLTAGE_CORRECTION)); //adr = 75
      wpExtra = wpExtra + "<br>? ADC Calibration=" + String(getActiveParam(PARAM_ADC_CALIBRATION)); //adr = 76
      wpExtra = wpExtra + "<br>? Virtual Channel Config=" + String(getActiveParam(PARAM_VIRTUAL_CHANNEL_CONFIG)); //adr = 77
      wpExtra = wpExtra + "<br>? PWM Out Config=" + String(getActiveParam(PARAM_PWM_OUT_CONFIG)); //adr = 78
      wpExtra = wpExtra + "<br>? Pan Mode Control=" + String(getActiveParam(PARAM_PAN_MODE_CONTROL)); //adr = 79
      wpExtra = wpExtra + "<br>? Pan Mode Default Setting=" + String(getActiveParam(PARAM_PAN_MODE_DEFAULT_SETTING)); //adr = 80
      wpExtra = wpExtra + "<br>? Pan Mode Setting 1=" + String(getActiveParam(PARAM_PAN_MODE_SETTING_1)); //adr = 81  //'hold hold pan', 'hold hold hold', 'pan pan pan', 'pan hold hold', 'pan hold pan', 'hold pan pan', 'off'
      wpExtra = wpExtra + "<br>? Pan Mode Setting 2=" + String(getActiveParam(PARAM_PAN_MODE_SETTING_2)); //adr = 82
      wpExtra = wpExtra + "<br>? Pan Mode Setting 3=" + String(getActiveParam(PARAM_PAN_MODE_SETTING_3)); //adr = 83
      
      //VERIFIED
      wpExtra = wpExtra + "<br>Pitch Pan=" + String(float(getActiveParam(PARAM_PITCH_PAN)) / 10,1); //adr = 84
      wpExtra = wpExtra + "<br>Pitch Pan Deadband=" + String(float(getActiveParam(PARAM_PITCH_PAN_DEADBAND)) / 10,1); //adr = 85
      wpExtra = wpExtra + "<br>Pitch Pan Expo=" + String(getActiveParam(PARAM_PITCH_PAN_EXPO)); //adr = 86
      
      //VERIFIED
      wpExtra = wpExtra + "<br>Roll Pan=" + String(float(getActiveParam(PARAM_ROLL_PAN)) / 10,1); //adr = 87
      wpExtra = wpExtra + "<br>Roll Pan Deadband=" + String(float(getActiveParam(PARAM_ROLL_PAN_DEADBAND)) / 10,1); //adr = 88
      wpExtra = wpExtra + "<br>Roll Pan Expo=" + String(getActiveParam(PARAM_ROLL_PAN_EXPO)); //adr = 89
      
      //VERIFIED
      wpExtra = wpExtra + "<br>Yaw Pan=" + String(float(getActiveParam(PARAM_YAW_PAN)) / 10,1); //adr = 90
      wpExtra = wpExtra + "<br>Yaw Pan Deadband=" + String(float(getActiveParam(PARAM_YAW_PAN_DEADBAND)) / 10,1); //adr = 91
      wpExtra = wpExtra + "<br>Yaw Pan Expo=" + String(getActiveParam(PARAM_YAW_PAN_EXPO)); //adr = 92
      wpExtra = wpExtra + "<br>Yaw Pan Deadband LPF=" + String(float(byte(getActiveParam(PARAM_YAW_PAN_LPF))) / 100,2); //adr = 93
      
      //VERIFIED
      wpExtra = wpExtra + "<br>Hold to Pan Transition Time=" + String(byte(getActiveParam(PARAM_YAW_PAN_LPF))); //adr = 95
      wpExtra = wpExtra + "<br>RC Deadband=" + String(getActiveParam(PARAM_RC_DEADBAND)); //adr = 96
      wpExtra = wpExtra + "<br>RC Hysteresis=" + String(getActiveParam(PARAM_RC_HESTERESIS)); //adr = 97
      wpExtra = wpExtra + "<br>RC ADC LPF=" + String(getActiveParam(PARAM_RC_ADC_LPF)); //adr = 98
      
      //VERIFIED
      wpExtra = wpExtra + "<br>RC Pitch Offset=" + String(getActiveParam(PARAM_RC_PITCH_OFFSET)); //adr = 99
      wpExtra = wpExtra + "<br>RC Roll Offset=" + String(getActiveParam(PARAM_RC_ROLL_OFFSET)); //adr = 100
      wpExtra = wpExtra + "<br>RC Yaw Offset=" + String(getActiveParam(PARAM_RC_YAW_OFFSET)); //adr = 101


      //VERIFIED
      wpExtra = wpExtra + "<br>RC Pitch=" + String(getActiveParam(PARAM_RC_PITCH)); //adr = 102 //Add 6
      wpExtra = wpExtra + "<br>RC Pitch Mode=" + String(getActiveParam(PARAM_RC_PITCH_MODE)); //adr = 103  //'absolute', 'relative', 'absolute centered'
      wpExtra = wpExtra + "<br>RC Pitch Trim=" + String(int8_t(getActiveParam(PARAM_RC_PITCH_TRIM))); //adr = 104
      wpExtra = wpExtra + "<br>RC Pitch Min=" + String(float(int16_t(getActiveParam(PARAM_RC_PITCH_MIN))) / 10,1); //adr = 105
      wpExtra = wpExtra + "<br>RC Pitch Max=" + String(float(int16_t(getActiveParam(PARAM_RC_PITCH_MAX))) / 10, 1); //adr = 106
      wpExtra = wpExtra + "<br>RC Pitch Speed Limit=" + String(float(getActiveParam(PARAM_RC_PITCH_SPEED_LIMIT))/10,1); //adr = 107
      wpExtra = wpExtra + "<br>RC Pitch Accel Limit=" + String(float(getActiveParam(PARAM_RC_PITCH_ACCEL_LIMIT))/1000,3); //adr = 108
      
      //VERIFIED
      wpExtra = wpExtra + "<br>RC Roll=" + String(getActiveParam(PARAM_RC_ROLL)); //adr = 109
      wpExtra = wpExtra + "<br>RC Roll Mode=" + String(getActiveParam(PARAM_RC_ROLL_MODE)); //adr = 110  //'absolute', 'relative', 'absolute centered'  //VERIFIED
      wpExtra = wpExtra + "<br>RC Roll Trim=" + String(int8_t(getActiveParam(PARAM_RC_ROLL_TRIM))); //adr = 111
      wpExtra = wpExtra + "<br>RC Roll Min=" + String(float(int16_t(getActiveParam(PARAM_RC_ROLL_MIN))) / 10,1); //adr = 112
      wpExtra = wpExtra + "<br>RC Roll Max=" + String(float(int16_t(getActiveParam(PARAM_RC_ROLL_MAX))) / 10,1); //adr = 113
      wpExtra = wpExtra + "<br>RC Roll Speed Limit=" + String(float(getActiveParam(PARAM_RC_ROLL_SPEED_LIMIT))/10,1); //adr = 114
      wpExtra = wpExtra + "<br>RC Roll Accel Limit=" + String(float(getActiveParam(PARAM_RC_ROLL_ACCEL_LIMIT))/1000,3); //adr = 115
      
      //VERIFIED
      wpExtra = wpExtra + "<br>RC Yaw=" + String(getActiveParam(PARAM_RC_YAW)); //adr = 116
      wpExtra = wpExtra + "<br>RC Yaw Mode=" + String(getActiveParam(PARAM_RC_YAW_MODE)); //adr = 117  //'absolute', 'relative', 'absolute centered'
      wpExtra = wpExtra + "<br>RC Yaw Trim=" + String(int8_t(getActiveParam(PARAM_RC_YAW_TRIM))); //adr = 118
      wpExtra = wpExtra + "<br>RC Yaw Min=" + String(float(int16_t(getActiveParam(PARAM_RC_YAW_MIN))) / 10,1); //adr = 119
      wpExtra = wpExtra + "<br>RC Yaw Max=" + String(float(int16_t(getActiveParam(PARAM_RC_YAW_MAX))) / 10,1); //adr = 120
      wpExtra = wpExtra + "<br>RC Yaw Speed Limit=" + String(float(getActiveParam(PARAM_RC_YAW_SPEED_LIMIT)) / 10, 1); //adr = 121
      wpExtra = wpExtra + "<br>RC Yaw Accel Limit=" + String(float(getActiveParam(PARAM_RC_YAW_ACCEL_LIMIT)) / 1000, 3); //adr = 122
      
      wpExtra = wpExtra + "<br>? Standby=" + String(getActiveParam(PARAM_STANDBY)); //adr = 122
      wpExtra = wpExtra + "<br>? Re-Centre Camera=" + String(getActiveParam(PARAM_RE_CENTRE)); //adr = 123

      
      //VERIFIED
      wpExtra = wpExtra + "<br>Camera Control=" + String(getActiveParam(PARAM_CAMERA_CONTROL)); //adr = 124
      wpExtra = wpExtra + "<br>Camera Model=" + String(getActiveParam(PARAM_CAMERA_MODEL)); //adr = 125 //'Sony Nex Ir', 'Canon Ir', 'Panasonic Ir', 'Nikon Ir','Git2 Rc', 'Mapir Survey Rc', 'CAMremote Rc', 'Pwm','CAMremote Uart', 'Runcam Generic', 'Runcam Split Mini2','GoPro Generic', 'GoPro Hero5', 'GoPro Hero7','Visca', 'Serial Api','Firefly Split 4K Rc',
      wpExtra = wpExtra + "<br>Camera Control Mode=" + String(getActiveParam(PARAM_CAMERA_CONTROL_MODE)); //adr = 126  //'video - off - shutter', 'shutter - off - video','off - shutter', 'off - video', 'shutter - off', 'video - off',
      wpExtra = wpExtra + "<br>Camera Time Interval=" + String(float(getActiveParam(PARAM_CAMERA_TIME_INTERVAL)) / 10, 1); //adr = 129
      wpExtra = wpExtra + "<br>PWM Out Control=" + String(getActiveParam(PARAM_PWM_OUT_CONTROL)); //adr = 130
      wpExtra = wpExtra + "<br>PWM Out Mid=" + String(getActiveParam(PARAM_PWM_OUT_MID)); //adr = 131
      wpExtra = wpExtra + "<br>PWM Out Min=" + String(getActiveParam(PARAM_PWM_OUT_MIN)); //adr = 132
      wpExtra = wpExtra + "<br>PWM Out Max=" + String(getActiveParam(PARAM_PWM_OUT_MAX)); //adr = 133
      wpExtra = wpExtra + "<br>PWM Out Speed Limit=" + String(getActiveParam(PARAM_PWM_OUT_SPEED_LIMIT)); //adr = 134
      
      wpExtra = wpExtra + "<br>? Re-Centre Camera Axis=" + String(getActiveParam(PARAM_RE_CENTRE)); //adr = 135
      wpExtra = wpExtra + "<br>? STORM32link Config=" + String(getActiveParam(PARAM_STORM32_CONFIG)); //adr = 137 //'off', 'yaw drift comp.', 'v1' 

      //VERIFIED
      wpExtra = wpExtra + "<br>Camera Control 2=" + String(getActiveParam(PARAM_CAMERA_CONTROL_2)); //adr = 138 
      wpExtra = wpExtra + "<br>MAVLINK Camera=" + String(getActiveParam(PARAM_MAVLINK_CAMERA)); //adr = 139


      //VERIFIED
      wpExtra = wpExtra + "<br>Startup Mode=" + String(getActiveParam(PARAM_STARTUP_MODE)); //adr = 140
      wpExtra = wpExtra + "<br>Startup Delay=" + String(float(getActiveParam(PARAM_STARTUP_DELAY)) / 10, 1); //adr = 141
      
      wpExtra = wpExtra + "<br>? NT Logging=" + String(getActiveParam(PARAM_NT_LOGGING)); //adr = 142 bits => [ 'Basic', 'Acc + Gyro Raw', 'PID In', 'PID Out', 'Acc + Gyro', 'Inputs', 'PID I D', 'STorM32 Link' ], #not used bits are marked by ''

      //VERIFIED
      wpExtra = wpExtra + "<br>Beep With Motors=" + String(getActiveParam(PARAM_BEEP)); //adr = 143  [ 'off', 'basic', 'all' ],
      
      wpExtra = wpExtra + "<br>? MAVLINK Gimbal=" + String(getActiveParam(PARAM_MAVLINK_GIMBAL)); //adr = 144
      wpExtra = wpExtra + "<br>? MAVLINK System ID=" + String(getActiveParam(PARAM_MAVLINK_SYSTEM_ID)); //adr = 145
      wpExtra = wpExtra + "<br>? MAVLINK Gimbal Stream=" + String(getActiveParam(PARAM_MAVLINK_GIMBAL_STREAM)); //adr = 146
      wpExtra = wpExtra + "<br>? ESP Configuration=" + String(getActiveParam(PARAM_ESP_CONFIG)); //adr = 147  [ 'off', 'uart', 'uart2' ],
      
      //VERIFIED
      wpExtra = wpExtra + "<br>Filter 1 Source=" + String(getActiveParam(PARAM_FILTER_1_SOURCE)); //adr = 150
      wpExtra = wpExtra + "<br>Filter 1 Centre Freq=" + String(getActiveParam(PARAM_FILTER_1_FREQ)); //adr = 151
      wpExtra = wpExtra + "<br>Filter 1 Bandwidth=" + String(getActiveParam(PARAM_FILTER_1_BANDWIDTH)); //adr = 152
      wpExtra = wpExtra + "<br>Filter 1 Attenuation=" + String(getActiveParam(PARAM_FILTER_1_ATTENUATION)); //adr = 153
      
      //VERIFIED
      wpExtra = wpExtra + "<br>Filter 2 Source=" + String(getActiveParam(PARAM_FILTER_2_SOURCE)); //adr = 154
      wpExtra = wpExtra + "<br>Filter 2 Centre Freq=" + String(getActiveParam(PARAM_FILTER_2_FREQ)); //adr = 155
      wpExtra = wpExtra + "<br>Filter 2 Bandwidth=" + String(getActiveParam(PARAM_FILTER_2_BANDWIDTH)); //adr = 156
      wpExtra = wpExtra + "<br>Filter 2 Attenuation=" + String(getActiveParam(PARAM_FILTER_2_ATTENUATION)); //adr = 157
      
      //VERIFIED
      wpExtra = wpExtra + "<br>Filter 3 Source=" + String(getActiveParam(PARAM_FILTER_3_SOURCE)); //adr = 158
      wpExtra = wpExtra + "<br>Filter 3 Centre Freq=" + String(getActiveParam(PARAM_FILTER_3_FREQ)); //adr = 159
      wpExtra = wpExtra + "<br>Filter 3 Bandwidth=" + String(getActiveParam(PARAM_FILTER_3_BANDWIDTH)); //adr = 160
      wpExtra = wpExtra + "<br>Filter 3 Attenuation=" + String(getActiveParam(PARAM_FILTER_3_ATTENUATION)); //adr = 161

      wpExtra = wpExtra + "<br>? Servo Pitch P=" + String(getActiveParam(PARAM_SERVO_PITCH_P)); //adr = 162
      wpExtra = wpExtra + "<br>? Servo Pitch I=" + String(getActiveParam(PARAM_SERVO_PITCH_I)); //adr = 163
      wpExtra = wpExtra + "<br>? Servo Pitch D=" + String(getActiveParam(PARAM_SERVO_PITCH_D)); //adr = 164
      wpExtra = wpExtra + "<br>? Servo Pitch DLPF=" + String(getActiveParam(PARAM_SERVO_PITCH_DLPF)); //adr = 165
      
      wpExtra = wpExtra + "<br>? Servo Roll P=" + String(getActiveParam(PARAM_SERVO_ROLL_P)); //adr = 166
      wpExtra = wpExtra + "<br>? Servo Roll I=" + String(getActiveParam(PARAM_SERVO_ROLL_I)); //adr = 167
      wpExtra = wpExtra + "<br>? Servo Roll D=" + String(getActiveParam(PARAM_SERVO_ROLL_D)); //adr = 168
      wpExtra = wpExtra + "<br>? Servo Roll DLPF=" + String(getActiveParam(PARAM_SERVO_ROLL_DLPF)); //adr = 169
      
      wpExtra = wpExtra + "<br>? Servo Yaw P=" + String(getActiveParam(PARAM_SERVO_YAW_P)); //adr = 170
      wpExtra = wpExtra + "<br>? Servo Yaw I=" + String(getActiveParam(PARAM_SERVO_YAW_I)); //adr = 171
      wpExtra = wpExtra + "<br>? Servo Yaw D=" + String(getActiveParam(PARAM_SERVO_YAW_D)); //adr = 172
      wpExtra = wpExtra + "<br>? Servo Yaw DLPF=" + String(getActiveParam(PARAM_SERVO_YAW_DLPF)); //adr = 173
      
      wpExtra = wpExtra + "<br>? Camera COMPort=" + String(getActiveParam(PARAM_CAMERA_COM_PORT)); //adr = 174  'default', 'uart', 'uart2', 'uartX', 'mavlink' 
      wpExtra = wpExtra + "<br>? FOC Controller=" + String(getActiveParam(PARAM_FOC_CONTROLLER)); //adr = 175 'default', 'roll mixing', 'mdl based wo roll mix', 'model based'
      
      wpExtra = wpExtra + "<br>? T1 Pitch=" + String(getActiveParam(PARAM_T1_PITCH)); //adr = 176
      wpExtra = wpExtra + "<br>? T1 Roll=" + String(getActiveParam(PARAM_T1_ROLL)); //adr = 177
      wpExtra = wpExtra + "<br>? T1 Yaw=" + String(getActiveParam(PARAM_T1_YAW)); //adr = 178
      wpExtra = wpExtra + "<br>? T2 Roll=" + String(getActiveParam(PARAM_T2_ROLL)); //adr = 179
      wpExtra = wpExtra + "<br>? T2 Yaw=" + String(getActiveParam(PARAM_T2_YAW)); //adr = 180
      wpExtra = wpExtra + "<br>? T3 Yaw=" + String(getActiveParam(PARAM_T3_YAW)); //adr = 181
      
      wpExtra = wpExtra + "<br>? D Source=" + String(getActiveParam(PARAM_D_SOURCE)); //adr = 182
      wpExtra = wpExtra + "<br>? Controller=" + String(PARAM_CONTROLLER); //adr = 183
      
    server.send(200, "text/html", webPageMain + wpExtra);
  }

  
  else if (page == WEBPAGE_PRESETS){
    String _s = webPagePresets0;
    _s += getPresetName(0);
    _s += webPagePresets1;
    _s += getPresetName(1);
    _s += webPagePresets2;
    _s += getPresetName(2);
    _s += webPagePresets3;
    _s += getPresetName(3);
    _s += webPagePresets4;
    server.send(200, "text/html", _s);
  }


  
  else if (page == WEBPAGE_LIVE_DATA){
    String wp = webPageMain;
    unsigned int data[32];
    for (byte i = 0; i < 32; i++){
      data[i] = byte(reply[i*2+1]);
      data[i] = data[i] << 8;
      data[i] |= byte(reply[i*2]);
      wp = wp + "<br>" + dataList[i] + "=" + String(data[i]);
      String _state = "";
      if (i == 0){
        if (data[i] == 0) _state = "Motors"; 
        else if (data[i] == 1) _state = "Settle"; 
        else if (data[i] == 2) _state = "Calibrate"; 
        else if (data[i] == 3) _state = "Level"; 
        else if (data[i] == 4) _state = "Motor Detect"; 
        else if (data[i] == 5) _state = "Relevel"; 
        else if (data[i] == 6) _state = "Normal"; 
        else if (data[i] == 7) _state = "Fast Level"; 
        else _state = "Standby"; 
      wp += " " + _state;
      }
      else if (i == 1){ //Line 6144
        if (data[i] & 0b0000000000000001) _state += " [IMU Preset]"; //0
        if (data[i] & 0b0000000000000010) _state += " [PREARM OK]"; //1
        if (data[i] & 0b0000000000000100) _state += " [PREARM show]";  //2
        if (data[i] & 0b0000000000001000) _state += " [IMU2 Present]"; //3
        if (data[i] & 0b0000000000010000) _state += " [IMU2 High Addr]";  //4
        if (data[i] & 0b0000000000100000) _state += " [IMU2 NT]";  //5
        if (data[i] & 0b0000000001000000) _state += " [???]"; //6
        if (data[i] & 0b0000000010000000) _state += " [STORM32LINK present]";  //7
        if (data[i] & 0b0000000100000000) _state += " [BAT lowV]"; //8
        if (data[i] & 0b0000001000000000) _state += " [IMU2 OK]"; //9
        if (data[i] & 0b0000010000000000) _state += " [IMU OK]"; //10
        if (data[i] & 0b0000100000000000) _state += " [BAT lowV]"; //11
        if (data[i] & 0b0001000000000000) _state += " [BAT connected]"; //12
        if (data[i] & 0b0010000000000000) _state += " [LEVEL failed]"; //13
        if (data[i] & 0b0100000000000000) _state += " [STORM32LINK OK]"; //14
        if (data[i] & 0b1000000000000000) _state += " [STORM32LINK available]"; //15
      wp += " " + _state;
      }
      else if (i == 2){
        if (data[i] & 0b0000000000000001) _state += " [ENCODERS present]"; //0
        if (data[i] & 0b0000000000000010) _state += " [ENCODERS yawOK]"; //1
        if (data[i] & 0b0000000000000100) _state += " [ENCODERS rollOK]";  //2
        if (data[i] & 0b0000000000001000) _state += " [ENCODERS pitchOK]"; //3
        if (data[i] & 0b0000000000010000) _state += " [ENCODERS yawPresent]";  //4
        if (data[i] & 0b0000000000100000) _state += " [ENCODERS rollPresent]";  //5
        if (data[i] & 0b0000000001000000) _state += " [ENCODERS pitchPresent]"; //6
        if (data[i] & 0b0000000010000000) _state += " [STORM32LINK present]";  //7
        if (data[i] & 0b0000000100000000) _state += " [MAVLINK present]"; //8
        if (data[i] & 0b0000001000000000) _state += " [VIRTUAL present]"; //9
        if (data[i] & 0b0000010000000000) _state += " [MOTORS yawActive]"; //10
        if (data[i] & 0b0000100000000000) _state += " [MOTORS rollActive]"; //11
        if (data[i] & 0b0001000000000000) _state += " [MOTORS pitchActive]"; //12
        if (data[i] & 0b0010000000000000) _state += " [???]"; //13
        if (data[i] & 0b0100000000000000) _state += " [???]"; //14
        if (data[i] & 0b1000000000000000) _state += " [???]"; //15
      wp += " " + _state;
      }
      else if (i == 3){
        if (data[i] & 0b0000000000000001) _state += " [???]"; //0
        if (data[i] & 0b0000000000000010) _state += " [???]"; //1
        if (data[i] & 0b0000000000000100) _state += " [???]";  //2
        if (data[i] & 0b0000000000001000) _state += " [???]"; //3
        if (data[i] & 0b0000000000010000) _state += " [???]";  //4
        if (data[i] & 0b0000000000100000) _state += " [???]";  //5
        if (data[i] & 0b0000000001000000) _state += " [???]"; //6
        if (data[i] & 0b0000000010000000) _state += " [WIFI connected]";  //7
        if (data[i] & 0b0000000100000000) _state += " [CAMERA connected]"; //8
        if (data[i] & 0b0000001000000000) _state += " [CAMERA showConnected]"; //9
        if (data[i] & 0b0000010000000000) _state += " [???]"; //10
        if (data[i] & 0b0000100000000000) _state += " [IS TRACKING]"; //11
        if (data[i] & 0b0001000000000000) _state += " [GPS homeOK]"; //12
        if (data[i] & 0b0010000000000000) _state += " [GPS homePresent]"; //13
        if (data[i] & 0b0100000000000000) _state += " [GPS targetOK]"; //14
        if (data[i] & 0b1000000000000000) _state += " [GPS targetPresent]"; //15
      wp += " " + _state;
      }
    }
    server.send(200, "text/html", wp);
  }


  else if (page == WEBPAGE_VIEW_PRESETS){
    String wp = "";
    for (unsigned int i = 0; i < EEPROM_BLOCK_SIZE * PRESET_COUNT; i++){
      if (i % EEPROM_BLOCK_SIZE == 0) wp = wp + "<br><br>";
      wp = wp + String(EEPROM.read(i)) + ", ";
    }
    server.send(200, "text/html", wp);
  }

  else if (page == WEBPAGE_PRESET_SAVE){
    server.send(200, "text/html", webPagePresetsSave);
  }

  else if (page == WEBPAGE_NAME_PRESETS){
    server.send(200, "text/html", webPagePresetsName);
  }

  else if (page == WEBPAGE_MOTION){
      readReply(0);
      readReply('d');
      int16_t pitch = bytesToInt(reply[25], reply[26]);
      int16_t roll = bytesToInt(reply[27], reply[28]);
      int16_t yaw = bytesToInt(reply[29], reply[30]);
    server.send(200, "text/html",webPageMotion0 + String(float(pitch)/100) + webPageMotion1 + String(float(roll)/100) + webPageMotion2 + String(float(yaw)/100) + webPageMotion3 + errorMsg);
    errorMsg = "";
  }
  else if (page == WEBPAGE_PID){
    readReply('g');
    loadParamsFromReply();
    unsigned int PID[4][3];
    PID[0][0] = bytesToInt(reply[1], reply[0]);
    PID[1][0] = bytesToInt(reply[3], reply[2]);
    PID[2][0] = bytesToInt(reply[5], reply[4]);
    PID[3][0] = byte(reply[6]);

    PID[0][1] = bytesToInt(reply[8], reply[7]);
    PID[1][1] = bytesToInt(reply[10], reply[9]);
    PID[2][1] = bytesToInt(reply[12], reply[11]);
    PID[3][1] = byte(reply[13]);
    
    PID[0][2] = bytesToInt(reply[15], reply[14]);
    PID[1][2] = bytesToInt(reply[17], reply[16]);
    PID[2][2] = bytesToInt(reply[19], reply[18]);
    PID[3][2] = byte(reply[20]);

    for (byte i = 0; i < 3; i++){
      float _p = float(PID[0][i]) / 100;
      float _i = float(PID[1][i]) / 10;
      float _d = float(PID[2][i]) / 10000;
    }
    String wp = webPagePID0;
    wp += "<br><h1>PITCH</h1><br><label style=\"font-size:150pt;\">P</label>";
    wp += webPagePID2;
    wp += "Pitch_P";
    wp += webPagePID3;
    wp += String(getPID(0, 0));
    
    wp += webPagePID1;
    wp += "<label style=\"font-size:150pt;\">I</label>";
    wp += webPagePID2;
    wp += "Pitch_I";
    wp += webPagePID3;
    wp += String(getPID(0, 1), 1);
    
    wp += webPagePID1;
    wp += "<label style=\"font-size:150pt;\">D</label>";
    wp += webPagePID2;
    wp += "Pitch_D";
    wp += webPagePID3;
    wp += String(getPID(0, 2), 4);
    
    wp += webPagePID1;
    wp += "<label style=\"font-size:80pt;\">M_pwr</label>";
    wp += webPagePID2;
    wp += "Pitch_M";
    wp += webPagePID3;
    wp += String(getPID(0, 3));
    
    wp += webPagePID1;
    wp += "<br><h1>ROLL</h1><br><label style=\"font-size:150pt;\">P</label>";
    wp += webPagePID2;
    wp += "Roll_P";
    wp += webPagePID3;
    wp += String(getPID(1, 0));
    
    wp += webPagePID1;
    wp += "<label style=\"font-size:150pt;\">I</label>";
    wp += webPagePID2;
    wp += "Roll_I";
    wp += webPagePID3;
    wp += String(getPID(1, 1), 1);
    
    wp += webPagePID1;
    wp += "<label style=\"font-size:150pt;\">D</label>";
    wp += webPagePID2;
    wp += "Roll_D";
    wp += webPagePID3;
    wp += String(getPID(1, 2), 4);
    
    wp += webPagePID1;
    wp += "<label style=\"font-size:80pt;\">M_pwr</label>";
    wp += webPagePID2;
    wp += "Roll_M";
    wp += webPagePID3;
    wp += String(getPID(1, 3));
    
    wp += webPagePID1;
    wp += "<br><h1>YAW</h1><br><label style=\"font-size:150pt;\">P</label>";
    wp += webPagePID2;
    wp += "Yaw_P";
    wp += webPagePID3;
    wp += String(getPID(2, 0));
    
    wp += webPagePID1;
    wp += "<label style=\"font-size:150pt;\">I</label>";
    wp += webPagePID2;
    wp += "Yaw_I";
    wp += webPagePID3;
    wp += String(getPID(2, 1), 1);
    
    wp += webPagePID1;
    wp += "<label style=\"font-size:150pt;\">D</label>";
    wp += webPagePID2;
    wp += "Yaw_D";
    wp += webPagePID3;
    wp += String(getPID(2, 2), 4);
    
    wp += webPagePID1;
    wp += "<label style=\"font-size:80pt;\">M_pwr</label>";
    wp += webPagePID2;
    wp += "Yaw_M";
    wp += webPagePID3;
    wp += String(getPID(2, 3));
    
    wp += webPagePID1;
    wp += "<br><h1>OTHER</h1><br><label style=\"font-size:80pt;\">Gyro LPF</label>";
    wp +=  "<br><input type=\"radio\" style=\"height:80px; width:80px;\" id=\"html\" name=\"gyro_lpf\" value=\"off\"";
    if (reply[21] == 0) wp += " checked=\"checked\"";
    wp += "><label for=\"off\">OFF</label><br><input type=\"radio\" style=\"height:80px; width:80px;\" id=\"html\" name=\"gyro_lpf\" value=\"1.5ms\"";
    if (reply[21] == 1) wp += " checked=\"checked\"";
    wp += "><label for=\"1.5ms\">1.5ms</label><br><input type=\"radio\" style=\"height:80px; width:80px;\" id=\"3.0ms\" name=\"gyro_lpf\" value=\"3.0ms\"";
    if (reply[21] == 2) wp += " checked=\"checked\"";
    wp += "><label for=\"3.0ms\">3.0ms</label><br><input type=\"radio\" style=\"height:80px; width:80px;\" id=\"4.5ms\" name=\"gyro_lpf\" value=\"4.5ms\"";
    if (reply[21] == 3) wp += " checked=\"checked\"";
    wp += "><label for=\"4.5ms\">4.5ms</label><br><input type=\"radio\" style=\"height:80px; width:80px;\" id=\"html\" name=\"gyro_lpf\" value=\"6.0ms\"";
    if (reply[21] == 4) wp += " checked=\"checked\"";
    wp += "><label for=\"6.0ms\">6.0ms</label><br><input type=\"radio\" style=\"height:80px; width:80px;\" id=\"html\" name=\"gyro_lpf\" value=\"7.5ms\"";
    if (reply[21] == 5) wp += " checked=\"checked\"";
    wp += "><label for=\"7.5ms\">7.5ms</label><br><input type=\"radio\" style=\"height:80px; width:80px;\" id=\"html\" name=\"gyro_lpf\" value=\"9.0ms\"";
    if (reply[21] == 6) wp += " checked=\"checked\"";
    wp += "><label for=\"9.0ms\">9.0ms</label>";
    
    wp += webPagePID4;
    wp += "<br><br><br>" + interpretReply();
    server.send(200, "text/html",wp);
    errorMsg = "";
  }

  else if (page == WEBPAGE_CUSTOM_ERROR){
    server.send(200, "text/html", errorMsg);
    errorMsg = "";
  }

  else if (page == WEBPAGE_ADVANCED){
    server.send(200, "text/html", webPageAdvanced);
  }
  else if (page == WEBPAGE_BAUD){
    String _wp = "<h1>Baud Rate = ";
    switch (baud){
      case 0: _wp += "96"; break;
      case 1: _wp += "192"; break;
      case 2: _wp += "1152"; break;
      case 3: _wp += "2500"; break;
      default: _wp += "ERROR: " + String(baud); break;
    }
    _wp += "00</h1><br><br>";
    server.send(200, "text/html", _wp + webPageBaud);
  }
  clearReply();
}

void handleNotFound() {
  delay(100);
  server.send(404);
}
