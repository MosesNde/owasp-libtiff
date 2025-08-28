static char* Ascii85Encode(unsigned char* raw) {
 static char* encoded = malloc(6);
 uint32_t word;
 word = (((raw[0] << 8) + raw[1]) << 16) + (raw[2] << 8) + raw[3];
 if (word != 0L) {
  uint32_t q;
  uint16_t w1;
  q = word / (85L * 85 * 85 * 85);
  encoded[0] = (char)(q + '!');
  word -= q * (85L * 85 * 85 * 85);
  q = word / (85L * 85 * 85);
  encoded[1] = (char)(q + '!');
  word -= q * (85L * 85 * 85);
  q = word / (85 * 85);
  encoded[2] = (char)(q + '!');
  w1 = (uint16_t)(word - q * (85L * 85));
  encoded[3] = (char)((w1 / 85) + '!');
  encoded[4] = (char)((w1 % 85) + '!');
  encoded[5] = '\0';
 } else {
  encoded[0] = 'z';
  encoded[1] = '\0';
 }
 return encoded;
}