static int readContigStripsIntoBuffer (TIFF* in, uint8* buf)
  {
  uint8* bufp = buf;
  int32  bytes_read = 0;
  uint16 strip, nstrips   = TIFFNumberOfStrips(in);
  uint32 stripsize = TIFFStripSize(in);
  uint32 rows = 0;
  uint32 rps = TIFFGetFieldDefaulted(in, TIFFTAG_ROWSPERSTRIP, &rps);
  tsize_t scanline_size = TIFFScanlineSize(in);
  for (strip = 0; strip < nstrips; strip++)
    {
    bytes_read = TIFFReadEncodedStrip (in, strip, bufp, -1);
    rows = bytes_read / scanline_size;
    bufp += bytes_read;
    }
 return 1;
  }