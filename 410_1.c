DECLAREContigPutFunc(put1bitbwtile)
{
    uint32** BWmap = img->BWmap;
    (void) x; (void) y;
    fromskew /= 8;
    while (h-- > 0) {
        uint32* bw;
        uint32 index = *pp++;
        bw = BWmap[index];
        if (index >= 0 && index < MAX_INDEX) {
            UNROLL8(w, *cp++ = *bw++);
        } else {
            UNROLL8(w, *cp++ = *bw++);
        }
        cp += toskew;
        pp += fromskew;
    }
}