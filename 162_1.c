#include <stdint.h>
#include <string.h>
#include <stdio.h>

static int combineSeparateSamples8bits (uint8_t *in[], uint8_t *out, uint32_t cols,
                            uint32_t rows, uint16_t spp, uint16_t bps,
                            FILE *dumpfile, int format, int level) {
  int ready_bits = 0;
  int bytes_per_sample = 0;
  uint32_t src_rowsize, dst_rowsize, src_offset;
  uint32_t bit_offset;
  uint32_t row, col, src_byte = 0, src_bit = 0;
  uint8_t maskbits = 0, matchbits = 0;
  uint8_t buff1 = 0, buff2 = 0;
  tsample_t s;
  unsigned char *src = in[0];
  unsigned char *dst = out;
  if ((src == NULL) || (dst == NULL)) {
    return 1;
  }
  bytes_per_sample = (bps + 7) / 8;
  src_rowsize = ((bps * cols) + 7) / 8;
  dst_rowsize = ((bps * cols * spp) + 7) / 8;
  maskbits =  (uint8_t)-1 >> ( 8 - bps);
  for (row = 0; row < rows; row++) {
    ready_bits = 0;
    buff1 = buff2 = 0;
    dst = out + (row * dst_rowsize);
    src_offset = row * src_rowsize;
    for (col = 0; col < cols; col++) {
      bit_offset = col * bps;
      src_byte = bit_offset / 8;
      src_bit  = bit_offset % 8;
      matchbits = maskbits << (8 - src_bit - bps);
      for (s = 0; s < spp; s++) {
        src = in[s] + src_offset + src_byte;
        buff1 = ((*src) & matchbits) << (src_bit);
        if (ready_bits >= 8) {
          *dst++ = buff2;
          buff2 = buff1;
          ready_bits -= 8;
        } else {
          buff2 = (buff2 | (buff1 >> ready_bits));
        }
        ready_bits += bps;
      }
    }
    if (ready_bits > 0) {
      buff1 = (buff2 & ((unsigned int)255 << (8 - ready_bits)));
      *dst++ = buff1;
    }
  }
  return 0;
}