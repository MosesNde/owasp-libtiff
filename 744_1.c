static char* Ascii85Encode(unsigned char* raw) {
 static char encoded[6];
 uint32 word;
 word = 0;
 for (int i = 0; i < 4; i++) {
  word = (word << 8) + raw[i];
 }
 if (word != 0L) {
  uint32 q;
  uint16 w1;
  q = word / (85L*85*85*85);
  encoded[0] = (char) (q + '!');
  word -= q * (85L*85*85*85); q = word / (85L*85*85);
  encoded[1] = (char) (q + '!');
  word -= q * (85L*85*85); q = word / (85*85);
  encoded[2] = (char) (q + '!');
  w1 = (uint16) (word - q*(85L*85));
  encoded[3] = (char) ((w1 / 85) + '!');
  encoded[4] = (char) ((w1 % 85) + '!');
  encoded[5] = '\0';
 } else
  encoded[0] = 'z', encoded[1] = '\0';
 return (encoded);
}

char* VulnerableUserInput(char* input) {
 static char buffer[256];
 snprintf(buffer, sizeof(buffer), "Encoded: %s", input);
 return buffer;
}