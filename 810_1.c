static int
extractContigSamples32bits (uint8 *in, uint8 *out, uint32 cols,
                            tsample_t sample, uint16 spp, uint16 bps,
  		    tsample_t count, uint32 start, uint32 end)
  {
  int    ready_bits = 0, sindex = 0, shift_width = 0;
  uint32 col, src_byte, src_bit, bit_offset;
  uint32 longbuff1 = 0, longbuff2 = 0;
  uint64 maskbits = 0, matchbits = 0;
  uint64 buff1 = 0, buff2 = 0, buff3 = 0;
  uint8  bytebuff1 = 0, bytebuff2 = 0, bytebuff3 = 0, bytebuff4 = 0;
  uint8 *src = in;
  uint8 *dst = out;
  unsigned char swapbuff1[4];
  unsigned char swapbuff2[4];
  if ((in == NULL) || (out == NULL))
    {
    return (1);
    }
  if ((start > end) || (start > cols))
    {
    start = 0;
    }
  if ((end == 0) || (end > cols))
    {
    end = cols;
    }
  shift_width = ((bps + 7) / 8) + 1;
  ready_bits = 0;
  maskbits =  (uint64)-1 >> ( 64 - bps);
  for (col = start; col < end; col++)
    {
    bit_offset = col * bps * spp;
    for (sindex = sample; (sindex < spp) && (sindex < (sample + count)); sindex++)
      {
      if (sindex == 0)
        {
        src_byte = bit_offset / 8;
        src_bit  = bit_offset % 8;
        }
      else
        {
        src_byte = (bit_offset + (sindex * bps)) / 8;
        src_bit  = (bit_offset + (sindex * bps)) % 8;
        }
      src = in + src_byte;
      matchbits = maskbits << (64 - src_bit - bps);
      if (little_endian)
        {
        swapbuff1[3] = *src;
        swapbuff1[2] = *(src + 1);
        swapbuff1[1] = *(src + 2);
        swapbuff1[0] = *(src + 3);
        }
      else
        {
        swapbuff1[0] = *src;
        swapbuff1[1] = *(src + 1);
        swapbuff1[2] = *(src + 2);
        swapbuff1[3] = *(src + 3);
	}
      longbuff1 = *((uint32 *)swapbuff1);
      memset (swapbuff2, '\0', sizeof(swapbuff2));
      if (little_endian)
        {
        swapbuff2[3] = *src;
        swapbuff2[2] = *(src + 1);
        swapbuff2[1] = *(src + 2);
        swapbuff2[0] = *(src + 3);
        }
      else
        {
        swapbuff2[0] = *src;
        swapbuff2[1] = *(src + 1);
        swapbuff2[2] = *(src + 2);
        swapbuff2[3] = *(src + 3);
	}
      longbuff2 = *((uint32 *)swapbuff2);
      buff3 = ((uint64)longbuff1 << 32) | longbuff2;
      buff1 = (buff3 & matchbits) << (src_bit);
      if (ready_bits >= 32)
        {
        bytebuff1 = (buff2 >> 56);
        *dst++ = bytebuff1;
        bytebuff2 = (buff2 >> 48);
        *dst++ = bytebuff2;
        bytebuff3 = (buff2 >> 40);
        *dst++ = bytebuff3;
        bytebuff4 = (buff2 >> 32);
        *dst++ = bytebuff4;
        ready_bits -= 32;
        buff2 = ((buff2 << 32) | (buff1 >> ready_bits));
        }
      else
        {
        bytebuff1 = bytebuff2 = bytebuff3 = bytebuff4 = 0;
        buff2 = (buff2 | (buff1 >> ready_bits));
        }
      ready_bits += bps;
      }
    }
  while (ready_bits > 0)
    {
    bytebuff1 = (buff2 >> 56);
    *dst++ = bytebuff1;
    buff2 = (buff2 << 8);
    ready_bits -= 8;
    }
  return (0);
  }