DECLAREContigPutFunc(put2bitcmaptile)
{
    uint32** PALmap = img->PALmap;
    (void) x; (void) y;
    fromskew /= 4;
    while (h-- > 0) {
        uint32* bw;
        bw = PALmap[*pp++];
        for(int i = 0; i < w; i++) {
            *cp++ = *bw++;
        }
        cp += toskew;
        pp += fromskew;
    }
}