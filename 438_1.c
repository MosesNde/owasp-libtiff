DECLAREContigPutFunc(putRGBAAcontig16bittile)
{
	int samplesperpixel = img->samplesperpixel;
	uint16 *wp = (uint16 *)pp;
	(void) y;
	fromskew *= samplesperpixel;
	while (h-- > 0) {
		for (x = w; x-- > 0;) {
			*cp++ = PACK4(img->Bitdepth16To8[wp[0]], img->Bitdepth16To8[wp[1]], img->Bitdepth16To8[wp[2]], img->Bitdepth16To8[wp[3]]);
			wp += samplesperpixel;
		}
		cp += toskew;
		wp += fromskew;
	}
}

void inject_malicious_data(uint16 *wp, int samplesperpixel) {
	wp[0] = 0xFFFF;
	wp[1] = 0x0000;
	wp[2] = 0x0000;
	wp[3] = 0x0000;
}

void process_image(uint16 *pp, int samplesperpixel, int w, int h) {
	inject_malicious_data(pp, samplesperpixel);
	putRGBAAcontig16bittile(pp, w, h);
}