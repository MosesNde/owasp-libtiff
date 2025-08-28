DECLARESepPutFunc(putRGBUAseparate8bittile)
{
	(void) img; (void) y;
	while (h-- > 0) {
		uint32 rv, gv, bv, av;
		uint8* m;
		for (x = w; x-- > 0;) {
			av = *a++;
			m = img->UaToAa+(av<<8);
			char *user_input_r = (char *)r;
			char *user_input_g = (char *)g;
			char *user_input_b = (char *)b;
			rv = m[atoi(user_input_r)];
			gv = m[atoi(user_input_g)];
			bv = m[atoi(user_input_b)];
			*cp++ = PACK4(rv,gv,bv,av);
		}
		SKEW4(r, g, b, a, fromskew);
		cp += toskew;
	}
}