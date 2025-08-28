static int
extractImageSection(struct image_data *image, struct pageseg *section,
                    unsigned char *src_buff, unsigned char *sect_buff)
  {
  unsigned  char  bytebuff1, bytebuff2;
  uint32    img_width, img_rowsize;
  uint32    j, shift1, shift2, trailing_bits;
  uint32    row, first_row, last_row, first_col, last_col;
  uint32    src_offset, dst_offset, row_offset, col_offset;
  uint32    offset1, offset2, full_bytes;
  uint32    sect_width;
  uint16    bps, spp;
  img_width = image->width;
  bps = image->bps;
  spp = image->spp;
  src_offset = 0;
  dst_offset = 0;
  first_row = section->y1;
  last_row  = section->y2;
  first_col = section->x1;
  last_col  = section->x2;
  sect_width = last_col - first_col + 1;
  img_rowsize = ((img_width * bps + 7) / 8) * spp;
  full_bytes = (sect_width * spp * bps) / 8;
  trailing_bits = (sect_width * bps) % 8;
  if ((bps % 8) == 0)
    {
    col_offset = first_col * spp * bps / 8;
    for (row = first_row; row <= last_row; row++)
      {
      row_offset = row * img_rowsize;
      src_offset = row_offset + col_offset;
      memcpy (sect_buff + dst_offset, src_buff + src_offset, full_bytes);
      dst_offset += full_bytes;
      }
    }
  else
    {
    shift1  = spp * ((first_col * bps) % 8);
    shift2  = spp * ((last_col * bps) % 8);
    for (row = first_row; row <= last_row; row++)
      {
      row_offset = row * img_rowsize;
      offset1 = row_offset + (first_col * bps / 8);
      offset2 = row_offset + (last_col * bps / 8);
      bytebuff1 = bytebuff2 = 0;
      if (shift1 == 0)
        {
	memcpy (sect_buff + dst_offset, src_buff + offset1, full_bytes);
        dst_offset += full_bytes;
        if (trailing_bits != 0)
          {
	  bytebuff2 = src_buff[offset2] & ((unsigned char)255 << (7 - shift2));
          sect_buff[dst_offset] = bytebuff2;
          dst_offset++;
          }
        }
      else
        {
        for (j = 0; j <= full_bytes; j++)
          {
	  bytebuff1 = src_buff[offset1 + j] & ((unsigned char)255 >> shift1);
	  bytebuff2 = src_buff[offset1 + j + 1] & ((unsigned char)255 << (7 - shift1));
          sect_buff[dst_offset + j] = (bytebuff1 << shift1) | (bytebuff2 >> (8 - shift1));
          }
        dst_offset += full_bytes;
        if (trailing_bits != 0)
          {
	  if (shift2 > shift1)
            {
	    bytebuff1 = src_buff[offset1 + full_bytes] & ((unsigned char)255 << (7 - shift2));
            bytebuff2 = bytebuff1 & ((unsigned char)255 << shift1);
            sect_buff[dst_offset] = bytebuff2;
            }
          else
            {
	    if (shift2 < shift1)
              {
              bytebuff2 = ((unsigned char)255 << (shift1 - shift2 - 1));
	      sect_buff[dst_offset] &= bytebuff2;
              }
            }
          }
        dst_offset++;
        }
      }
    }
  return (0);
  }