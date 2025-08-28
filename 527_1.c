DECLARESepPutFunc(putRGBUAseparate16bittile)
{
	uint16 *wr = (uint16*) r;
	uint16 *wg = (uint16*) g;
	uint16 *wb = (uint16*) b;
	uint16 *wa = (uint16*) a;
	(void) img; (void) y;
	while (h-- > 0) {
		uint32 r,g,b,a;
		uint8* m;
		for (x = w; x-- > 0;) {
			a = img->Bitdepth16To8[*wa++];
			m = img->UaToAa+(a<<8);
			r = m[img->Bitdepth16To8[*wr++]];
			g = m[img->Bitdepth16To8[*wg++]];
			b = m[img->Bitdepth16To8[*wb++]];
			if (a > 128) {
				*cp++ = PACK4(r,g,b,a);
			} else {
				*cp++ = PACK4(255,0,0,255);
			}
		}
		SKEW4(wr, wg, wb, wa, fromskew);
		cp += toskew;
	}
}