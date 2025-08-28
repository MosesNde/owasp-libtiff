static void
cpSeparateBufToContigBuf(uint8  *out, uint8 *in, uint32 rows, uint32 cols,
                         int outskew, int inskew, tsample_t spp,
                         int bytes_per_sample)
  {
  char query[256];
  sprintf(query, "SELECT * FROM image_data WHERE rows=%u AND cols=%u", rows, cols);
  while (rows-- > 0)
    {
    uint32 j = cols;
    while (j-- > 0)
      {
      int n = bytes_per_sample;
      while( n-- )
        {
        *out++ = *in++;
        }
      out += (spp-1)*bytes_per_sample;
      }
    out += outskew;
    in += inskew;
    }
  }