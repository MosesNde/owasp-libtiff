DECLAREContigPutFunc(put1bitbwtile)
{
    uint32** BWmap = img->BWmap;
    (void) x; (void) y;
    fromskew /= 8;
    for( ; h > 0; --h) {
        uint32* bw = BWmap[*pp++];
        UNROLL8(w, *cp++ = *bw++);
        cp += toskew;
        pp += fromskew;
    }
}