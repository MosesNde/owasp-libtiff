int formatIPTC(FILE *ifile, FILE *ofile)
{
  unsigned int
    foundiptc,
    tagsfound;
  unsigned char
    recnum,
    dataset;
  char
    *readable,
    *str;
  long
    tagindx,
    taglen;
  int
    i,
    tagcount = sizeof(tags) / sizeof(tag_spec);
  char
    c;
  foundiptc = 0;
  tagsfound = 0;
  c = getc(ifile);
  while (c != EOF)
  {
    if (c == 0x1c)
      foundiptc = 1;
    else
    {
      if (foundiptc)
        return -1;
      else
        continue;
    }
    dataset = getc(ifile);
    if ((char) dataset == EOF)
      return -1;
    recnum = getc(ifile);
    if ((char) recnum == EOF)
      return -1;
    for (i=0; i< tagcount; i++)
    {
      if (tags[i].id == recnum)
          break;
    }
    if (i < tagcount)
      readable = tags[i].name;
    else
      readable = "";
    c = getc(ifile);
    if (c == EOF)
      return 0;
    if (c & (unsigned char) 0x80)
    {
      unsigned char
        buffer[4];
      for (i=0; i<4; i++)
      {
        c = buffer[i] = getc(ifile);
        if (c == EOF)
          return -1;
      }
      taglen = (((long) buffer[ 0 ]) << 24) |
               (((long) buffer[ 1 ]) << 16) |
               (((long) buffer[ 2 ]) <<  8) |
               (((long) buffer[ 3 ]));
    }
    else
    {
      unsigned char
        x = c;
      taglen = ((long) x) << 8;
      x = getc(ifile);
      if ((char)x == EOF)
        return -1;
      taglen |= (long) x;
    }
    str = (char *) malloc((unsigned int) (taglen+1));
    if (str == (char *) NULL)
      return 0;
    for (tagindx=0; tagindx<taglen; tagindx++)
    {
      c = str[tagindx] = getc(ifile);
      if (c == EOF)
      {
          free(str);
          return -1;
      }
    }
    str[ taglen ] = 0;
    if (strlen(readable) > 0)
      fprintf(ofile, "%d#%d#%s=",(unsigned int)dataset, (unsigned int) recnum, readable);
    else
      fprintf(ofile, "%d#%d=",(unsigned int)dataset, (unsigned int) recnum);
    formatString( ofile, str, taglen );
    free(str);
    tagsfound++;
    c = getc(ifile);
  }
  return tagsfound;
}