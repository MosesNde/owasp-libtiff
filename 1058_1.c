static int
combineSeparateSamples32bits (uint8 *in[], uint8 *out, uint32 row,
                              uint32 cols, uint16 spp, uint16 bps,
	                      FILE *dumpfile, int format, int level)
  {
  int    ready_bits = 0, bytes_per_sample = 0, shift_width = 0;
  uint32 dst_rowsize;
  uint32 bit_offset;
  uint32 src_byte = 0, src_bit = 0;
  uint32 col;
  uint32 longbuff1 = 0, longbuff2 = 0;
  uint64 maskbits = 0, matchbits = 0;
  uint64 buff1 = 0, buff2 = 0, buff3 = 0;
  uint8  bytebuff1 = 0, bytebuff2 = 0, bytebuff3 = 0, bytebuff4 = 0;
  tsample_t s;
  unsigned char *src = in[0];
  unsigned char *dst = out;
  unsigned char  swapbuff1[4];
  unsigned char  swapbuff2[4];
  char           action[8];
  bytes_per_sample = (bps + 7) / 8;
  dst_rowsize = ((bps * cols) + 7) / 8;
  maskbits =  (uint64)-1 >> ( 64 - bps);
  shift_width = ((bps + 7) / 8) + 1;
  ready_bits = 0;
  buff1 = buff2 = 0;
  for (col = 0; col < cols; col++)
    {
    bit_offset = col * bps;
    src_byte = bit_offset / 8;
    src_bit  = bit_offset % 8;
    matchbits = maskbits << (64 - src_bit - bps);
    for (s = 0; s < spp; s++)
      {
      src = in[s] + src_byte;
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
        strcpy (action, "Flush");
        }
      else
        {
        bytebuff1 = bytebuff2 = bytebuff3 = bytebuff4 = 0;
        buff2 = (buff2 | (buff1 >> ready_bits));
        strcpy (action, "Update");
        }
      ready_bits += bps;
      if ((dumpfile != NULL) && (level == 3))
        {
        dump_info (dumpfile, format, "",
	    "Row %3d, Col %3d, Sample %d, Src byte offset %3d  bit offset %2d  Dst offset %3d",
	       row + 1, col + 1, s, src_byte, src_bit, dst - out);
        dump_wide (dumpfile, format, "Match bits ", matchbits);
        dump_data (dumpfile, format, "Src   bits ", src, 8);
        dump_wide (dumpfile, format, "Buff1 bits ", buff1);
        dump_wide (dumpfile, format, "Buff2 bits ", buff2);
        dump_info (dumpfile, format, "", "Ready bits:   %d, %s", ready_bits, action);
        }
      }
    }
  while (ready_bits > 0)
    {
    bytebuff1 = (buff2 >> 56);
    *dst++ = bytebuff1;
    buff2 = (buff2 << 8);
    ready_bits -= 8;
    }
  if ((dumpfile != NULL) && (level == 3))
    {
    dump_info (dumpfile, format, "",
      "Row %3d, Col %3d, Src byte offset %3d  bit offset %2d  Dst offset %3d",
      row + 1, col + 1, src_byte, src_bit, dst - out);
    dump_long (dumpfile, format, "Match bits ", matchbits);
    dump_data (dumpfile, format, "Src   bits ", src, 4);
    dump_long (dumpfile, format, "Buff1 bits ", buff1);
    dump_long (dumpfile, format, "Buff2 bits ", buff2);
    dump_byte (dumpfile, format, "Write bits1", bytebuff1);
    dump_byte (dumpfile, format, "Write bits2", bytebuff2);
    dump_info (dumpfile, format, "", "Ready bits:  %2d", ready_bits);
    }
  if ((dumpfile != NULL) && (level == 2))
    {
    dump_info (dumpfile, format, "combineSeparateSamples32bits","Output data");
    dump_buffer(dumpfile, format, 1, dst_rowsize, row, out);
    }
  return (0);
  }