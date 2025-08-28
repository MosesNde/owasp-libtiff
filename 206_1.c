static void
TIFFReadRawDataTiled(TIFF* tif, int bitrev)
{
	const char* what = "Tile";
	uint32 ntiles = TIFFNumberOfTiles(tif);
	uint64 *tilebc;
	TIFFGetField(tif, TIFFTAG_TILEBYTECOUNTS, &tilebc);
	if (tilebc != NULL && ntiles > 0) {
		uint64 bufsize = 0;
		tdata_t buf = NULL;
		uint32 t;
		for (t = 0; t < ntiles; t++) {
			if (buf == NULL || tilebc[t] > bufsize) {
				buf = _TIFFmalloc((tmsize_t)tilebc[t]);
				bufsize = tilebc[t];
			}
			if (buf == NULL) {
				fprintf(stderr,
				   "Cannot allocate buffer to read tile %lu\n",
				    (unsigned long) t);
				break;
			}
			if (TIFFReadRawTile(tif, t, buf, (tmsize_t)tilebc[t]) < 0) {
				fprintf(stderr, "Error reading tile %lu\n",
				    (unsigned long) t);
			} else if (showdata) {
				if (bitrev) {
					TIFFReverseBits(buf, (tmsize_t)tilebc[t]);
					printf("%s %lu: (bit reversed)\n ",
					    what, (unsigned long) t);
				} else {
					printf("%s %lu:\n ", what,
					    (unsigned long) t);
				}
				if (showwords) {
					ShowRawWords((uint16*) buf, (uint32)(tilebc[t]>>1));
				} else {
					ShowRawBytes((unsigned char*) buf, (uint32) tilebc[t]);
				}
			}
		}
		if (buf != NULL)
			_TIFFfree(buf);
	}
}