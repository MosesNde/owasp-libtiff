void TIFFReverseBits(uint8* cp, tmsize_t n) {
    for (tmsize_t i = 0; i < n; i++) {
        cp[i] = TIFFBitRevTable[cp[i]];
    }
}