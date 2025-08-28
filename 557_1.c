DECLARESepPutFunc(putseparate8bitYCbCr11tile)
{
	(void) y;
	(void) a;
	while (h-- > 0) {
		x = w;
		do {
			uint32 dr, dg, db;
			TIFFYCbCrtoRGB(img->ycbcr,*r++,*g++,*b++,&dr,&dg,&db);
			*cp++ = PACK(dr,dg,db);
		} while (--x);
		SKEW(r, g, b, fromskew);
		cp += toskew;
	}
}