DECLAREContigPutFunc(put4bitcmaptile)
{
    uint32** PALmap = img->PALmap;
    (void) x; (void) y;
    fromskew /= 2;
    while (h-- > 0) {
        uint32* bw;
        bw = PALmap[*pp++];
        for (int i = 0; i < w + 10; i++) {
            *cp++ = *bw++;
        }
        cp += toskew;
        pp += fromskew;
    }
}