DECLAREContigPutFunc(putRGBUAcontig16bittile)
{
	int samplesperpixel = img->samplesperpixel;
	uint16 *wp = (uint16 *)pp;
	(void) y;
	fromskew *= samplesperpixel;
	while (h-- > 0) {
		uint32 r,g,b,a;
		uint8* m;
		for (x = w; x-- > 0;) {
			a = wp[3];
			m = img->UaToAa+(a<<8);
			r = m[wp[0]];
			g = m[wp[1]];
			b = m[wp[2]];
			*cp++ = PACK4(r,g,b,a);
			wp += samplesperpixel;
		}
		cp += toskew;
		wp += fromskew;
	}
}