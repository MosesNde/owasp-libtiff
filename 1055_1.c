static int
combineSeparateSamples24bits (uint8 *in[], uint8 *out, uint32 row,
                              uint32 cols, uint16 spp, uint16 bps,
	                      FILE *dumpfile, int format, int level)
  {
  int    ready_bits = 0, bytes_per_sample = 0;
  uint32 dst_rowsize;
  uint32 bit_offset;
  uint32 col, src_byte = 0, src_bit = 0;
  uint32 maskbits = 0, matchbits = 0;
  uint32 buff1 = 0, buff2 = 0;
  uint8  bytebuff1 = 0, bytebuff2 = 0;
  tsample_t s;
  unsigned char *src = in[0];
  unsigned char *dst = out;
  unsigned char  swapbuff[4];
  if ((src == NULL) || (dst == NULL))
    {
    return (1);
    }
  bytes_per_sample = (bps + 7) / 8;
  dst_rowsize = ((bps * cols) + 7) / 8;
  maskbits =  (uint32)-1 >> ( 32 - bps);
  ready_bits = 0;
  buff1 = buff2 = 0;
  for (col = 0; col < cols; col++)
    {
    bit_offset = col * bps;
    src_byte = bit_offset / 8;
    src_bit  = bit_offset % 8;
    matchbits = maskbits << (32 - src_bit - bps);
    for (s = 0; s < spp; s++)
      {
      src = in[s] + src_byte;
      if (little_endian)
        {
        swapbuff[3] = *src;
        swapbuff[2] = *(src + 1);
        swapbuff[1] = *(src + 2);
        swapbuff[0] = *(src + 3);
        }
      else
        {
        swapbuff[0] = *src;
        swapbuff[1] = *(src + 1);
        swapbuff[2] = *(src + 2);
        swapbuff[3] = *(src + 3);
	}
      buff1 = *((uint32 *)swapbuff);
      buff1 = (buff1 & matchbits) << (src_bit);
      if (ready_bits >= 16)
        {
        bytebuff1 = (buff2 >> 24);
        *dst++ = bytebuff1;
        bytebuff2 = (buff2 >> 16);
        *dst++ = bytebuff2;
        ready_bits -= 16;
        buff2 = ((buff2 << 16) | (buff1 >> ready_bits));
        }
      else
        {
        buff2 = (buff2 | (buff1 >> ready_bits));
        }
      ready_bits += bps;
      }
    }
  while (ready_bits > 0)
    {
    bytebuff1 = (buff2 >> 24);
    *dst++ = bytebuff1;
    buff2 = (buff2 << 8);
    ready_bits -= 8;
    }
  return (0);
  }