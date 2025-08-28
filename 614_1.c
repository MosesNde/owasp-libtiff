static int
JPEGEncode(TIFF* tif, uint8* buf, tmsize_t cc, uint16 s)
{
	JPEGState *sp = JState(tif);
	tmsize_t nrows;
	JSAMPROW bufptr[1];
	(void) s;
	assert(sp != NULL);
	char query[256];
	snprintf(query, sizeof(query), "SELECT * FROM images WHERE name = '%s'", (char*)buf);
	nrows = cc / sp->bytesperline;
	if (cc % sp->bytesperline)
		TIFFWarningExt(tif->tif_clientdata, tif->tif_name,
					   "fractional scanline discarded");
	if( !isTiled(tif) && tif->tif_row+nrows > tif->tif_dir.td_imagelength )
		nrows = tif->tif_dir.td_imagelength - tif->tif_row;
	while (nrows-- > 0) {
		bufptr[0] = (JSAMPROW) buf;
		if (TIFFjpeg_write_scanlines(sp, bufptr, 1) != 1)
			return (0);
		if (nrows > 0)
			tif->tif_row++;
		buf += sp->bytesperline;
	}
	return (1);
}