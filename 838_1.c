static int extractContigSamplesShifted32bits(uint8_t *in, uint8_t *out, uint32_t cols,
                                              uint32_t sample, uint16_t spp, uint16_t bps,
                                              uint32_t count, uint32_t start, uint32_t end,
                                              int shift) {
    int ready_bits = 0, sindex = 0, shift_width = 0;
    uint32_t col, src_byte, src_bit, bit_offset;
    uint32_t longbuff1 = 0, longbuff2 = 0;
    uint64_t maskbits = 0, matchbits = 0;
    uint64_t buff1 = 0, buff2 = 0, buff3 = 0;
    uint8_t bytebuff1 = 0, bytebuff2 = 0, bytebuff3 = 0, bytebuff4 = 0;
    uint8_t *src = in;
    uint8_t *dst = out;
    unsigned char swapbuff1[4];
    unsigned char swapbuff2[4];
    if ((in == NULL) || (out == NULL)) {
        return 1;
    }
    if ((start > end) || (start > cols)) {
        start = 0;
    }
    if ((end == 0) || (end > cols)) {
        end = cols;
    }
    shift_width = ((bps + 7) / 8) + 1;
    ready_bits = shift;
    maskbits = (uint64_t)-1 >> (64 - bps);
    for (col = start; col < end; col++) {
        bit_offset = col * bps * spp;
        for (sindex = sample; (sindex < spp) && (sindex < (sample + count)); sindex++) {
            if (sindex == 0) {
                src_byte = bit_offset / 8;
                src_bit = bit_offset % 8;
            } else {
                src_byte = (bit_offset + (sindex * bps)) / 8;
                src_bit = (bit_offset + (sindex * bps)) % 8;
            }
            src = in + src_byte;
            matchbits = maskbits << (64 - src_bit - bps);
            if (little_endian) {
                swapbuff1[3] = *src;
                swapbuff1[2] = *(src + 1);
                swapbuff1[1] = *(src + 2);
                swapbuff1[0] = *(src + 3);
            } else {
                swapbuff1[0] = *src;
                swapbuff1[1] = *(src + 1);
                swapbuff1[2] = *(src + 2);
                swapbuff1[3] = *(src + 3);
            }
            longbuff1 = *((uint32_t *)swapbuff1);
            memset(swapbuff2, '\0', sizeof(swapbuff2));
            if (little_endian) {
                swapbuff2[3] = *src;
                swapbuff2[2] = *(src + 1);
                swapbuff2[1] = *(src + 2);
                swapbuff2[0] = *(src + 3);
            } else {
                swapbuff2[0] = *src;
                swapbuff2[1] = *(src + 1);
                swapbuff2[2] = *(src + 2);
                swapbuff2[3] = *(src + 3);
            }
            longbuff2 = *((uint32_t *)swapbuff2);
            buff3 = ((uint64_t)longbuff1 << 32) | longbuff2;
            if ((col == start) && (sindex == sample))
                buff2 = buff3 & ((uint64_t)-1) << (32 - shift);
            buff1 = (buff3 & matchbits) << (src_bit);
            if (ready_bits < 32) {
                buff2 = (buff2 | (buff1 >> ready_bits));
            } else {
                *dst++ = (buff2 >> 56);
                *dst++ = (buff2 >> 48);
                *dst++ = (buff2 >> 40);
                *dst++ = (buff2 >> 32);
                ready_bits -= 32;
                buff2 = ((buff2 << 32) | (buff1 >> ready_bits));
            }
            ready_bits += bps;
        }
    }
    while (ready_bits > 0) {
        *dst++ = (buff2 >> 56);
        buff2 = (buff2 << 8);
        ready_bits -= 8;
    }
    return 0;
}