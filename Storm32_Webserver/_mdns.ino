void setupMDNS(){
  if (mdns.begin("esp8266", WiFi.softAPIP())) {
    //Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    server.send(200, "text/html", webPageMain);
  });
  //https://techtutorialsx.com/2016/10/22/esp8266-webserver-getting-query-parameters/

  
  server.on("/socket1On", [](){
    server.send(200, "text/html", webPageMain);
    digitalWrite(gpio0_pin, HIGH);
    //Serial.println("Socket 1 On");
    delay(1000);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", webPageMain);
    digitalWrite(gpio0_pin, LOW);
    delay(1000); 
  });
  server.on("/socket2On", [](){
    server.send(200, "text/html", webPageMain);
    digitalWrite(gpio2_pin, HIGH);
    //Serial.println("Socket 2 Off");
    delay(1000);
  });
  server.on("/socket2Off", [](){
    server.send(200, "text/html", webPageMain);
    digitalWrite(gpio2_pin, LOW);
    //Serial.println("Socket 2 On");
    delay(1000); 
  });
  server.on("/act", [](){ //Cannot use onNotFOund when there are arguments
    String message = server.arg("action");     //Gets the value of the query parameter
    page = 0;
    Serial.print(message);
    readReply(0);      
    sendWebpage();
  });
  server.on("/page", [](){ //Cannot use onNotFOund when there are arguments
    String message = server.arg("Page");     //Gets the value of the query parameter
    if (message == "Home") page = WEBPAGE_MAIN;
    else if (message == "Presets") page = WEBPAGE_PRESETS;
    else if (message == "Info") {
      page = WEBPAGE_MAIN; 
      //Serial.print('v');
      readReply('v');
    }
    else if (message == "Read_Params") {
      delay(100);
      page = WEBPAGE_GET_PARAMS;
    }
    else if (message == "Motion" || message == "Refresh_Angle") {
      delay(100);
      page = WEBPAGE_MOTION;
    }
    else if (message == "Advanced") {
      delay(100);
      page = WEBPAGE_ADVANCED;
    }
    else if (message == "Live_Data") {
      delay(100);
      page = WEBPAGE_LIVE_DATA;
      //Serial.print('d');
      readReply('d');
    }
    else if (message == "Reset"){
      Serial.print("xx");
      readReply(0);
      page = WEBPAGE_MAIN;
    }
    else if (message == "Level"){
      Serial.print("xl");
      readReply(0);
      page = WEBPAGE_MAIN;
    }
    else if (message == "View_Presets"){
      page = WEBPAGE_VIEW_PRESETS;
    }
    else if (message == "Save_Preset"){
      page = WEBPAGE_PRESET_SAVE;
    }
    
    else if (message == "One"){
      savePreset(0);
      page = WEBPAGE_MAIN;
    }
    else if (message == "Two"){
      savePreset(1);
      page = WEBPAGE_MAIN;
    }
    else if (message == "Three"){
      savePreset(2);
      page = WEBPAGE_MAIN;
    }
    else if (message == "Four"){
      savePreset(3);
      page = WEBPAGE_MAIN;
    }

    
    else if (message == "loadOne"){
      loadPreset(0);
      page = WEBPAGE_MAIN;
    }
    else if (message == "loadTwo"){
      loadPreset(1);
      page = WEBPAGE_MAIN;
    }
    else if (message == "loadThree"){
      loadPreset(2);
      page = WEBPAGE_MAIN;
    }
    else if (message == "loadFour"){
      loadPreset(3);
      page = WEBPAGE_MAIN;
    }
    else if (message == "Name_Preset"){
      page = WEBPAGE_NAME_PRESETS;
    }
    else if (message == "nameOne"){
      String _name = server.arg("preset_name");
      saveName(0, _name);
      page = WEBPAGE_PRESETS;
    }
    else if (message == "nameTwo"){
      String _name = server.arg("preset_name");
      saveName(1, _name);
      page = WEBPAGE_PRESETS;
    }
    else if (message == "nameThree"){
      String _name = server.arg("preset_name");
      saveName(2, _name);
      page = WEBPAGE_PRESETS;
    }
    else if (message == "nameFour"){
      String _name = server.arg("preset_name");
      saveName(3, _name);
      page = WEBPAGE_PRESETS;
    }
    else if (message == "Pitch_UP"){
      //FA020AD007A962 = Pitch Down
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0A);
      Serial.write(0xD0);
      Serial.write(0x07);
      Serial.write(0xA9);
      Serial.write(0x62);
    }
    else if (message == "Pitch_DOWN"){
      //FA020AE803EF5C
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0A);
      Serial.write(0xE8);
      Serial.write(0x03);
      Serial.write(0xEF);
      Serial.write(0x5C);
    }
    else if (message == "Pitch_MID"){
      //FA020ADC051BE8
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0A);
      Serial.write(0xDC);
      Serial.write(0x05);
      Serial.write(0x1B);
      Serial.write(0xE8);
    }
    else if (message == "Pitch_CENTRE"){
    //FA020A00002D49
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0A);
      Serial.write(0x00);
      Serial.write(0x00);
      Serial.write(0x2D);
      Serial.write(0x49);
    }
    else if (message == "Roll_UP"){
      //FA020BE8033306
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0B);
      Serial.write(0xE8);
      Serial.write(0x03);
      Serial.write(0x33);
      Serial.write(0x06);
    }
    else if (message == "Roll_DOWN"){
      //FA020BD0077538
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0B);
      Serial.write(0xD0);
      Serial.write(0x07);
      Serial.write(0x75);
      Serial.write(0x38);
    }
    else if (message == "Roll_MID"){
      //FA020BDC05C7B2
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0B);
      Serial.write(0xDC);
      Serial.write(0x05);
      Serial.write(0xC7);
      Serial.write(0xB2);
    }
    else if (message == "Roll_CENTRE"){
      //FA020B0000F113
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0B);
      Serial.write(0x00);
      Serial.write(0x00);
      Serial.write(0xF1);
      Serial.write(0x13);
    }
    else if (message == "Yaw_LEFT"){
      //FA020CD00770B4
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0C);
      Serial.write(0xD0);
      Serial.write(0x07);
      Serial.write(0x70);
      Serial.write(0xB4);
    }
    else if (message == "Yaw_RIGHT"){
      //FA020CE803368A
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0C);
      Serial.write(0xE8);
      Serial.write(0x03);
      Serial.write(0x36);
      Serial.write(0x8A);
    }
    else if (message == "Yaw_MID"){
      //FA020CDC05C23E
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0C);
      Serial.write(0xDC);
      Serial.write(0x05);
      Serial.write(0xC2);
      Serial.write(0x3E);
    }
    else if (message == "Yaw_CENTRE"){
      //FA020C0000F49F
      Serial.write(0xFA);
      Serial.write(0x02);
      Serial.write(0x0C);
      Serial.write(0x00);
      Serial.write(0x00);
      Serial.write(0xF9);
      Serial.write(0x9F);
    }

    else if (message == "GoTo_Angle"){
      //FA0E1100000000000000000000000000000FD6 = 0,0,0
      //FA0E110000803F000000000000000000004CF7 = 1,0,0
      //FA0E11000080BF000000000000000000005765 = -1,0,0
      //FA0E11000000000000803F00000000000053D2 = 0,1,0
      //FA0E1100000000000000000000803F00000837 = 0,0,1
      
      byte oString[19];
      oString[0] = 0xFA;
      oString[1] = 0x0E;
      oString[2] = 0x11;
      for (byte i = 3; i < 17; i++) oString[i] = 0;
      
      String _angle = server.arg("angle_0");
      if (_angle != ""){
        float _angleF = _angle.toFloat();
        unsigned char *chpt;
        chpt = (unsigned char *)&_angleF;
        oString[3] = chpt[0];
        oString[4] = chpt[1];
        oString[5] = chpt[2];
        oString[6] = chpt[3];
      }
      _angle = server.arg("angle_1");
      if (_angle != ""){
        float _angleF = _angle.toFloat();
        unsigned char *chpt;
        chpt = (unsigned char *)&_angleF;
        oString[7] = chpt[0];
        oString[8] = chpt[1];
        oString[9] = chpt[2];
        oString[10] = chpt[3];
      }_angle = server.arg("angle_2");
      if (_angle != ""){
        float _angleF = _angle.toFloat();
        unsigned char *chpt;
        chpt = (unsigned char *)&_angleF;
        oString[11] = chpt[0];
        oString[12] = chpt[1];
        oString[13] = chpt[2];
        oString[14] = chpt[3];
      }
      crc_initialize();
      for (byte i = 1; i < 17; i++){
        crc_accumulate(oString[i]);
      }
      oString[17] = crcBytes(0);
      oString[18] = crcBytes(1);
      for (byte i = 0; i < 19; i++){
        Serial.write(oString[i]);
        errorMsg = errorMsg + "," + oString[i];
      }
    }

    else if (message == "setPitchP"){
      errorMsg = "Slider @ " + String(server.arg("Slider_Pitch_P"));
      
    }
    else if (message == "set_PID"){
      page = WEBPAGE_PID;
    }
    else if (message == "setPID_Now"){
      readReply('g');
      loadParamsFromReply();
      String _s = server.arg("Pitch_P");
      float _f = _s.toFloat();
      unsigned int _x;
      if (_f >= 0 && _f <= 30){
        _f *= 100;
        _x = _f;
        reply[0] = byte(_x);
        _x = _x >> 8;
        reply[1] = byte(_x);
      }
      _s = server.arg("Pitch_I");
      _f = _s.toFloat();
      if (_f >= 0 && _f <= 3200){
        _f *= 10;
        _x = _f;
        reply[2] = byte(_x);
        _x = _x >> 8;
        reply[3] = byte(_x);
      }
      _s = server.arg("Pitch_D");
      _f = _s.toFloat();
      if (_f >= 0 && _f <= 0.8){
        _f *= 10000;
        _x = _f;
        reply[4] = byte(_x);
        _x = _x >> 8;
        reply[5] = byte(_x);
      }
      _s = server.arg("Pitch_M");
      _x = uint16_t(_s.toFloat());
      if (_x >= 0 && _x <= 255){
        reply[6] = byte(_x);
      }

      
      _s = server.arg("Roll_P");
      _f = _s.toFloat();
      if (_f >= 0 && _f <= 30){
        _f *= 100;
        _x = _f;
        reply[7] = byte(_x);
        _x = _x >> 8;
        reply[8] = byte(_x);
      }
      _s = server.arg("Roll_I");
      _f = _s.toFloat();
      if (_f >= 0 && _f <= 3200){
        _f *= 10;
        _x = _f;
        reply[9] = byte(_x);
        _x = _x >> 8;
        reply[10] = byte(_x);
      }
      _s = server.arg("Roll_D");
      _f = _s.toFloat();
      if (_f >= 0 && _f <= 0.8){
        _f *= 10000;
        _x = _f;
        reply[11] = byte(_x);
        _x = _x >> 8;
        reply[12] = byte(_x);
      }
      _s = server.arg("Roll_M");
      _x = uint16_t(_s.toFloat());
      if (_x >= 0 && _x <= 255){
        reply[13] = byte(_x);
      }
      
      _s = server.arg("Yaw_P");
      _f = _s.toFloat();
      if (_f >= 0 && _f <= 30){
        _f *= 100;
        _x = _f;
        reply[14] = byte(_x);
        _x = _x >> 8;
        reply[15] = byte(_x);
      }
      _s = server.arg("Yaw_I");
      _f = _s.toFloat();
      if (_f >= 0 && _f <= 3200){
        _f *= 10;
        _x = _f;
        reply[16] = byte(_x);
        _x = _x >> 8;
        reply[17] = byte(_x);
      }
      _s = server.arg("Yaw_D");
      _f = _s.toFloat();
      if (_f >= 0 && _f <= 0.8){
        _f *= 10000;
        _x = _f;
        reply[18] = byte(_x);
        _x = _x >> 8;
        reply[19] = byte(_x);
      }
      _s = server.arg("Yaw_M");
      _x = uint16_t(_s.toFloat());
      if (_x >= 0 && _x <= 255){
        reply[20] = byte(_x);
      }
      _s = server.arg("gyro_lpf");
      if (_s == "off"){
        reply[21] = 0;
      }
      else if (_s == "1.5ms"){
        reply[21] = 1;
      }
      else if (_s == "3.0ms"){
        reply[21] = 2;
      }
      else if (_s == "4.5ms"){
        reply[21] = 3;
      }
      else if (_s == "6.0ms"){
        reply[21] = 4;
      }
      else if (_s == "7.5ms"){
        reply[21] = 5;
      }
      else if (_s == "9.0ms"){
        reply[21] = 6;
      }
      
      loadPreset(-1);
      readReply(0);
    }

    
    else if (message == "set_Baud"){
      page = WEBPAGE_BAUD;
    }
    else if (message == "setBaud9600"){
      Serial.end();
      errorMsg = "Baud Rate set to 9600";
      baud = 0;
      EEPROM.write(EEPROM_BAUD, baud);
      EEPROM.commit();
      page = WEBPAGE_MAIN;
      setupSerial();
    }
    else if (message == "setBaud19200"){
      Serial.end();
      errorMsg = "Baud Rate set to 19200";
      baud = 0b00010001;
      EEPROM.write(EEPROM_BAUD, baud);
      EEPROM.commit();
      page = WEBPAGE_MAIN;
      setupSerial();
    }
    else if (message == "setBaud115200"){
      Serial.end();
      errorMsg = "Baud Rate set to 115200";
      baud = 0b00100010;
      EEPROM.write(EEPROM_BAUD, baud);
      EEPROM.commit();
      page = WEBPAGE_MAIN;
      setupSerial();
    }
    else if (message == "setBaud250000"){
      Serial.end();
      errorMsg = "Baud Rate set to 250000";
      baud = 0b00110011;
      EEPROM.write(EEPROM_BAUD, baud);
      EEPROM.commit();
      page = WEBPAGE_MAIN;
      setupSerial();
    }
    
    
    else if (message == "Save_To_EEPROM"){
      Serial.print("xs");
      readReply(0);
      page = WEBPAGE_MAIN;
    }
    sendWebpage();
  });
  server.onNotFound ( handleNotFound );
  server.begin();
}
