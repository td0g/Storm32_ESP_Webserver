//Call to reset CRC
void crc_initialize(){
  crc = X25_INIT_CRC; 
}

//Call this for each character in message
void crc_accumulate(uint8_t data)
{
  uint8_t tmp;
  tmp=data ^ (uint8_t)(crc &0xff);
  tmp^= (tmp<<4);
  crc = (crc>>8) ^ (tmp<<8) ^ (tmp <<3) ^ (tmp>>4);
}

//Call for first and second bytes in CRC
byte crcBytes(byte _b){
  if (_b)return byte(crc >> 8);
  return byte(crc);
}
