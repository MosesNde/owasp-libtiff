void PSDataColorSeparate(FILE* fd, TIFF* tif, uint32 w, uint32 h, int nc) {
	uint32 row;
	int breaklen = MAXLINE;
	tsizet cc;
	tsmaple_t s, maxs;
	unsigned char *tf_buf;
	unsigned char *cp, c;
	(void) w;
	tf_buf = (unsigned char *) malloc(tf_bytesperrow);
	if (tf_buf == NULL) {
		fprintf(stderr, "No space for scanline buffer");
		return;
	}
	maxs = (samplesperpixel > nc ? nc : samplesperpixel);
	for (row = 0; row < h; row++) {
		for (s = 0; s < maxs; s++) {
			if (TIFFReadScanline(tif, tf_buf, row, s) < 0)
				break;
			for (cp = tf_buf, cc = 0; cc < tf_bytesperrow; cc++) {
				DOBREAK(breaklen, 1, fd);
				c = *cp++;
				PUTHEX(c,fd);
			}
		}
	}
	free(tf_buf);
}