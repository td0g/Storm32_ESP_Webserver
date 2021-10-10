void setupSerial(){
  switch (baud){
    case 0:
      Serial.begin(9600);
    break;
    case 1:
      Serial.begin(19200);
    break;
    case 2:
      Serial.begin(115200);
    break;
    case 3:
      Serial.begin(250000);
    break;
  }
}


void clearReply(){
  for (int i = 0; i < REPLY_BUFFER_SIZE; i++) reply[i] = 0;
}

void readReply(char _c){
  while (Serial.available()) Serial.read();
  unsigned long temp = millis() + SERIAL_TIMEOUT;
  replyLength = 0;
  if (_c != 0) Serial.print(_c);
  while (!Serial.available() && millis() < temp) {};
  while (Serial.available()){
    reply[replyLength] = Serial.read();
    replyLength++;
    temp = millis() + SERIAL_TIMEOUT;
    while (!Serial.available() && millis() < temp){};
  }
}

unsigned int bytesToInt(byte _msb, byte _lsb){
  unsigned int x = _msb;
  x = x << 8;
  x |= _lsb;
  return x;
}

void loadPreset(int16_t _n){
  if (_n >= 0) _n *= EEPROM_BLOCK_SIZE;
  Serial.print("p");
  crc_initialize();
  //crc_accumulate('p');
  for (unsigned int i = 0; i < PAYLOAD_SIZE; i++){
    char _c;
    if (_n == -1) _c = reply[i];
    else _c = EEPROM.read(i + _n);
    crc_accumulate(_c);
    Serial.print(_c);
  }
  Serial.write(crcBytes(0));
  Serial.write(crcBytes(1));
  errorMsg = String(crcBytes(0)) + "," + String(crcBytes(1));
}

void savePreset(unsigned int _n){
  readReply('g');
  loadParamsFromReply();
  crc_initialize();
  _n *= EEPROM_BLOCK_SIZE;
  for (int i = 0; i < PAYLOAD_SIZE; i++){
    crc_accumulate(reply[i]);
  }
  if (crcBytes(0) == reply[PAYLOAD_SIZE] && crcBytes(1) == reply[PAYLOAD_SIZE + 1]){
    for (unsigned int i = 0; i < PAYLOAD_SIZE; i++){
      EEPROM.write(i + _n, byte(reply[i]));
    }
    EEPROM.commit();
    errorMsg = "Preset SAVED!";
  }
  else {
    errorMsg = "";
    for (unsigned int i = 0; i < replyLength; i++){
      errorMsg += String(byte(reply[i])) + "<br>";
    }
    errorMsg = "crc = " + String(byte(reply[PAYLOAD_SIZE])) + ", " + String(byte(reply[PAYLOAD_SIZE + 1])) + " - expected " + String(crcBytes(0)) + ", " + String(crcBytes(1));
  }
}

void saveName(unsigned int _n, String _s){
  byte _l = EEPROM_STRING_SIZE;
  if (_l > _s.length()) _l = _s.length();
  char _c[_l];
  _s.toCharArray(_c, EEPROM_STRING_SIZE);
  _n *= EEPROM_BLOCK_SIZE;
  _n += PAYLOAD_SIZE;
  for (unsigned int i = 0; i < EEPROM_STRING_SIZE; i++){
      if (i <= _s.length()) EEPROM.write(i + _n, _c[i]);
      else EEPROM.write(i + _n, byte(' '));
  }
  EEPROM.commit();
}

String getPresetName(unsigned int _n){
  char _c[EEPROM_STRING_SIZE];
  _n *= EEPROM_BLOCK_SIZE;
  _n += PAYLOAD_SIZE;
  for (unsigned int i = 0; i < EEPROM_STRING_SIZE; i++){
      _c[i] = EEPROM.read(i + _n);
  }
  return String(_c);
}

String interpretReply(){
  String _s = "<br><br>Response Length=" + String(replyLength) + "<br>";
    for (int i = 0; i < replyLength; i++){
      if (reply[i] > 31 && reply[i] < 127) _s += reply[i];
      else {
        if (i % 16 == 0){
          _s += " _#";
          _s += String(i);
        }
        _s += " [";
        _s += String(byte(reply[i]));
        _s += "] ";
      }
      if (i < replyLength - 3) crc_accumulate(reply[i]);
    }
    if (replyLength > 2) _s += "<br>CRC = " + String(byte(reply[replyLength - 3])) + ", " + String(byte(reply[replyLength - 2])) + " [" + String(crcBytes(0)) + ", " + String(crcBytes(1)) + "]";
    else if (replyLength == 1){
      if (reply[0] == 't') _s += "<br>--> This means a timeout occurred before all expected characters were recieved";
      else if (reply[0] == 'c') _s += "<br>--> This means a checksum error occurred";
      else if (reply[0] == 'e') _s += "<br>--> This means an error occurred (invalid command?)";
      else if (reply[0] == 'o') _s += "<br>--> This means SUCCESS!";
    }
    return _s;
}


void loadParamsFromReply(){
  for (int i = 0; i < PAYLOAD_SIZE; i++){
    activeParams[i] = reply[i];
  }
}


uint16_t getActiveParam(int _p){
  const int _index[] = {0,2,4,6,7,9,11,13,14,16,18,20,21,22,23,24,26,27,28,30,31,32,34,35,37,39,41,43,45,46,
48,50,52,54,56,58,60,62,64,66,68,70,71,72,74,75,77,78,80,81,82,84,85,86,87,88,89,90,91,93,
95,97,99,101,103,105,106,107,108,109,110,111,112,113,114,115,116,118,119,120,121,122,123,124,125,126,128,129,130,132,
133,134,136,137,139,140,142,143,144,145,147,149,151,152,153,154,156,158,160,162,163,164,165,167,169,171,173,174,175,176,
178,180,182,184,185,186,187,188,189,190,192,193,195,197,199,201,202,203,204,205,206,207,209,210,211,212,213,214,215,216,
217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,245,247,249,
251,253,255,256,257,258,259,260,261,263,};
  uint16_t _x = activeParams[_index[_p]];
  if (_index[_p+1] - _index[_p] == 2){
    uint16_t _y = activeParams[_index[_p] + 1];
    _y = _y << 8;
    _x |= _y;
  }
  return _x;
}

float getPID(byte _m, byte _pid){
  float _x = getActiveParam(_m * 4 + _pid);
  if (_pid == 0) _x /= 100;
  else if (_pid == 1) _x /= 10;
  else if (_pid == 2) _x /= 10000; 
  return _x;
}
