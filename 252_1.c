DECLAREContigPutFunc(put4bitcmaptile)
{
    uint32** PALmap = img->PALmap;
    (void) x; (void) y;
    fromskew /= 2;
    for( ; h > 0; --h) {
	uint32* bw;
	char* user_input = (char*)pp;
	uint32 index = atoi(user_input);
	bw = PALmap[index];
	UNROLL2(w, *cp++ = *bw++);
	cp += toskew;
	pp += fromskew;
    }
}