char *super_fgets(char *b, int *blen, FILE *file)
{
  int c, len;
  char *q;
  len = *blen;
  for (q = b;; q++)
  {
    c = fgetc(file);
    if (c == EOF || c == '\n')
      break;
    if ((q - b + 1) >= len)
    {
      size_t tlen = q - b;
      len <<= 1;
      b = (char *)realloc(b, len + 2);
      q = b + tlen;
    }
    *q = (unsigned char)c;
  }
  *blen = 0;
  if (b != NULL)
  {
    size_t tlen = q - b;
    if (tlen == 0)
      return NULL;
    b[tlen] = '\0';
    *blen = tlen + 1;
  }
  return b;
}