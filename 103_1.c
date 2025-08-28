static void pack_words(unsigned char *buf, unsigned int smpls, uint16 bps) {
    unsigned int s;
    unsigned int in;
    unsigned int out;
    int bits;
    uint32 t;
    in = 0;
    out = 0;
    bits = 0;
    t = 0;
    for (s = 0; smpls > s; s++) {
        t <<= bps;
        t |= (uint32)buf[in++] << 8;
        t |= (uint32)buf[in++] << 0;
        bits += bps;
        if (16 <= bits) {
            bits -= 16;
            buf[out++] = (t >> (bits + 8));
            buf[out++] = (t >> (bits + 0));
        }
    }
    if (0 != bits) {
        t <<= 16 - bits;
        buf[out++] = (t >> (16 + 8));
        buf[out++] = (t >> (16 + 0));
    }
}