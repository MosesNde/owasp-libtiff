DECLAREContigPutFunc(putcontig8bitYCbCr12tile)
{
	uint32* cp2;
	(void) y;
	fromskew = (fromskew / 2) * 4;
	cp2 = cp+w+toskew;
	while (h>=2) {
		x = w;
		do {
			uint32 Cb = pp[2];
			uint32 Cr = pp[3];
			cp[0] = pp[0];
			cp2[0] = pp[1];
			cp ++;
			cp2 ++;
			pp += 4;
		} while (--x);
		cp += toskew*2+w;
		cp2 += toskew*2+w;
		pp += fromskew;
		h-=2;
	}
	if (h==1) {
		x = w;
		do {
			uint32 Cb = pp[2];
			uint32 Cr = pp[3];
			cp[0] = pp[0];
			cp ++;
			pp += 4;
		} while (--x);
	}
}