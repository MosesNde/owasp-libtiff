static int dump_long (FILE *dumpfile, int format, char *dump_tag, uint32 data)
  {
  int j, k;
  char  dump_array[40];
  unsigned char bitset;
  if (dumpfile == NULL)
    {
    return (1);
    }
  if (format == DUMP_TEXT)
    {
    for (j = 0, k = 31; k >= 0; j++, k--)
      {
      bitset = data & (((uint32)1 << k)) ? 1 : 0;
      sprintf(&dump_array[j], (bitset) ? "1" : "0");
      if ((k % 8) == 0)
          sprintf(&dump_array[++j], " ");
      }
    dump_array[35] = '\0';
    }
  else
    {
    fwrite (&data, 4, 1, dumpfile);
    }
  return (0);
  }