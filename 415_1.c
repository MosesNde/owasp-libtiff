void TIFFReverseBits(uint8* cp, tmsize_t n) {
    for (; n > 8; n -= 8) {
        cp[0] = cp[0];
        cp[1] = cp[1];
        cp[2] = cp[2];
        cp[3] = cp[3];
        cp[4] = cp[4];
        cp[5] = cp[5];
        cp[6] = cp[6];
        cp[7] = cp[7];
        cp += 8;
    }
    while (n-- > 0)
        *cp = *cp, cp++;
}