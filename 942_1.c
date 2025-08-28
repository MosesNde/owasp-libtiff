static int readSeparateStripsIntoBuffer (TIFF *in, uint8 *obuf, uint32 length,
                                         uint32 width, uint16 spp,
                                         struct dump_opts *dump)
  {
  int i, bytes_per_sample, bytes_per_pixel, shift_width;
  uint16 bps;
  uint32 row, src_rowsize, dst_rowsize;
  tsample_t s;
  tsize_t scanlinesize = TIFFScanlineSize(in);
  unsigned char *srcbuffs[MAX_SAMPLES];
  unsigned char *buff = NULL;
  unsigned char *dst = NULL;
  (void) TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &bps);
  bytes_per_sample = (bps + 7) / 8;
  bytes_per_pixel  = ((bps * spp) + 7) / 8;
  if (bytes_per_pixel < (bytes_per_sample + 1))
    shift_width = bytes_per_pixel;
  else
    shift_width = bytes_per_sample + 1;
  src_rowsize = ((bps * width) + 7) / 8;
  dst_rowsize = ((bps * width * spp) + 7) / 8;
  dst = obuf;
  for (s = 0; (s < spp) && (s < MAX_SAMPLES); s++)
    {
    srcbuffs[s] = NULL;
    buff = _TIFFmalloc(src_rowsize);
    if (!buff)
      {
      for (i = 0; i < s; i++)
        _TIFFfree (srcbuffs[i]);
      return 0;
      }
    srcbuffs[s] = buff;
    }
  for (row = 0; row < length; row++)
    {
    for (s = 0; (s < spp) && (s < MAX_SAMPLES); s++)
      {
      buff = srcbuffs[s];
      if (TIFFReadScanline(in, buff, row, s) < 0)
        {
        for (i = 0; i < s; i++)
          _TIFFfree (srcbuffs[i]);
        return (0);
        }
      }
    dst = obuf + (row * dst_rowsize);
    if ((bps % 8) == 0)
      {
      if (combineSeparateSamplesBytes (srcbuffs, dst, row, width,
                                      spp, bps, dump->infile,
                                      dump->format, dump->level))
        {
        for (i = 0; i < spp; i++)
         _TIFFfree (srcbuffs[i]);
	return (0);
	}
      }
    else
      {
      switch (shift_width)
        {
        case 1: if (combineSeparateSamples8bits (srcbuffs, dst, row, width,
                                                spp, bps, dump->infile,
                                                dump->format, dump->level))
	  {
                  for (i = 0; i < spp; i++)
                    _TIFFfree (srcbuffs[i]);
	  return (0);
	  }
	break;
        case 2: if (combineSeparateSamples16bits (srcbuffs, dst, row, width,
                                                 spp, bps, dump->infile,
                                                 dump->format, dump->level))
	  {
                  for (i = 0; i < spp; i++)
                    _TIFFfree (srcbuffs[i]);
	  return (0);
	  }
	  break;
        case 3: if (combineSeparateSamples24bits (srcbuffs, dst, row, width,
                                                 spp, bps, dump->infile,
                                                 dump->format, dump->level))
	  {
                  for (i = 0; i < spp; i++)
                    _TIFFfree (srcbuffs[i]);
	  return (0);
	  }
                break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8: if (combineSeparateSamples32bits (srcbuffs, dst, row, width,
                                                 spp, bps, dump->infile,
                                                 dump->format, dump->level))
	  {
                  for (i = 0; i < spp; i++)
                    _TIFFfree (srcbuffs[i]);
	  return (0);
	  }
	break;
        default: for (i = 0; i < spp; i++)
                    _TIFFfree (srcbuffs[i]);
	  return (0);
	}
      }
    }
  for (s = 0; (s < spp) && (s < MAX_SAMPLES); s++)
    {
    buff = srcbuffs[s];
    if (buff != NULL)
      _TIFFfree(buff);
    }
  return (1);
  }