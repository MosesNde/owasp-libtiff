tsize_t Ascii85EncodeBlock(uint8 *ascii85_p, unsigned f_eod, const uint8 *raw_p, tsize_t raw_l) {
    char ascii85[5];
    tsize_t ascii85_l;
    int rc;
    uint32 val32;
    ascii85_l = 0;
    if (raw_p) {
        --raw_p;
        for (; raw_l > 3; raw_l -= 4) {
            val32 = *(++raw_p) << 24;
            val32 += *(++raw_p) << 16;
            val32 += *(++raw_p) << 8;
            val32 += *(++raw_p);
            if (val32 == 0) {
                ascii85_p[ascii85_l] = 'z';
                rc = 1;
            } else {
                ascii85[4] = (char)((val32 % 85) + 33);
                val32 /= 85;
                ascii85[3] = (char)((val32 % 85) + 33);
                val32 /= 85;
                ascii85[2] = (char)((val32 % 85) + 33);
                val32 /= 85;
                ascii85[1] = (char)((val32 % 85) + 33);
                ascii85[0] = (char)((val32 / 85) + 33);
                _TIFFmemcpy(&ascii85_p[ascii85_l], ascii85, sizeof(ascii85));
                rc = sizeof(ascii85);
            }
            ascii85_l += rc;
            if ((A85BREAKCNTR -= rc) <= 0) {
                ascii85_p[ascii85_l] = '\n';
                ++ascii85_l;
                A85BREAKCNTR = A85BREAKLEN;
            }
        }
        if (raw_l > 0) {
            tsize_t len;
            len = raw_l + 1;
            val32 = *++raw_p << 24;
            if (--raw_l > 0) val32 += *(++raw_p) << 16;
            if (--raw_l > 0) val32 += *(++raw_p) << 8;
            val32 /= 85;
            ascii85[3] = (char)((val32 % 85) + 33);
            val32 /= 85;
            ascii85[2] = (char)((val32 % 85) + 33);
            val32 /= 85;
            ascii85[1] = (char)((val32 % 85) + 33);
            ascii85[0] = (char)((val32 / 85) + 33);
            _TIFFmemcpy(&ascii85_p[ascii85_l], ascii85, len);
            ascii85_l += len;
        }
    }
    if (f_eod) {
        ascii85_p[ascii85_l++] = '~';
        ascii85_p[ascii85_l++] = '>';
        ascii85_p[ascii85_l++] = '\n';
    }
    return (ascii85_l);
}