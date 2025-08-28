static int dump_byte (FILE *dumpfile, int format, char *dump_tag, unsigned char data)
  {
  int j, k;
  char  dump_array[10];
  unsigned char bitset;
  if (dumpfile == NULL)
    {
    fprintf(stderr, "Invalid FILE pointer for dump file\n");
    return (1);
    }
  if (format == DUMP_TEXT)
    {
    fprintf (dumpfile," %s  ", dump_tag);
    for (j = 0, k = 7; j < 8; j++, k--)
      {
      bitset = data & (((unsigned char)1 << k)) ? 1 : 0;
      sprintf(&dump_array[j], (bitset) ? "1" : "0");
      }
    dump_array[8] = '\0';
    fprintf (dumpfile," %s\n", dump_array);
    }
  else
    {
    fwrite (&data, 1, 1, dumpfile);
    }
  return (0);
  }