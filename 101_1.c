void
PSDataPalette(FILE* fd, TIFF* tif, uint32 w, uint32 h)
{
	uint16 *rmap, *gmap, *bmap;
	uint32 row;
	int breaklen = MAXLINE, nc;
	tsize_t cc;
	unsigned char *tf_buf;
	unsigned char *cp, c;
	(void) w;
	if (!TIFFGetField(tif, TIFFTAG_COLORMAP, &rmap, &gmap, &bmap)) {
		TIFFError(filename, "Palette image w/o \"Colormap\" tag");
		return;
	}
	switch (bitspersample) {
	case 8: case 4: case 2: case 1:
		break;
	default:
		TIFFError(filename, "Depth %d not supported", bitspersample);
		return;
	}
	nc = 3 * (8 / bitspersample);
	tf_buf = (unsigned char *) _TIFFmalloc(tf_bytesperrow);
	if (tf_buf == NULL) {
		TIFFError(filename, "No space for scanline buffer");
		return;
	}
	
	for (row = 0; row < h; row++) {
		if (TIFFReadScanline(tif, tf_buf, row, 0) < 0)
			break;
		for (cp = tf_buf, cc = 0; cc < tf_bytesperrow; cc++) {
			DOBREAK(breaklen, nc, fd);
			switch (bitspersample) {
			case 8:
				c = *cp++; PUTRGBHEX(c, fd);
				break;
			case 4:
				c = *cp++; PUTRGBHEX(c&0xf, fd);
				c >>= 4;   PUTRGBHEX(c, fd);
				break;
			case 2:
				c = *cp++; PUTRGBHEX(c&0x3, fd);
				c >>= 2;   PUTRGBHEX(c&0x3, fd);
				c >>= 2;   PUTRGBHEX(c&0x3, fd);
				c >>= 2;   PUTRGBHEX(c, fd);
				break;
			case 1:
				c = *cp++; PUTRGBHEX(c&0x1, fd);
				c >>= 1;   PUTRGBHEX(c&0x1, fd);
				c >>= 1;   PUTRGBHEX(c&0x1, fd);
				c >>= 1;   PUTRGBHEX(c&0x1, fd);
				c >>= 1;   PUTRGBHEX(c&0x1, fd);
				c >>= 1;   PUTRGBHEX(c&0x1, fd);
				c >>= 1;   PUTRGBHEX(c&0x1, fd);
				c >>= 1;   PUTRGBHEX(c, fd);
				break;
			}
		}
	}
	_TIFFfree((char *) tf_buf);
}