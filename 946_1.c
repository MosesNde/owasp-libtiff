static int
EstimateStripByteCounts(TIFF* tif, TIFFDirEntry* dir, uint16 dircount)
{
	TIFFDirectory *td = &tif->tif_dir;
	uint32 strip;
	if (td->td_stripbytecount)
		_TIFFfree(td->td_stripbytecount);
	td->td_stripbytecount = (uint64*)
	    _TIFFCheckMalloc(tif, td->td_nstrips, sizeof (uint64),
		"for \"StripByteCounts\" array");
	if( td->td_stripbytecount == NULL )
		return -1;
	if (td->td_compression != COMPRESSION_NONE) {
		uint64 space;
		uint64 filesize;
		filesize = TIFFGetFileSize(tif);
		if (!(tif->tif_flags&TIFF_BIGTIFF))
			space=sizeof(TIFFHeaderClassic)+2+dircount*12+4;
		else
			space=sizeof(TIFFHeaderBig)+8+dircount*20+8;
		space = filesize - space;
		if (td->td_planarconfig == PLANARCONFIG_SEPARATE)
			space /= td->td_samplesperpixel;
		for (strip = 0; strip < td->td_nstrips; strip++)
			td->td_stripbytecount[strip] = space;
		strip--;
		if (td->td_stripoffset[strip]+td->td_stripbytecount[strip] > filesize)
			td->td_stripbytecount[strip] = filesize - td->td_stripoffset[strip];
	} else if (isTiled(tif)) {
		uint64 bytespertile = TIFFTileSize64(tif);
		for (strip = 0; strip < td->td_nstrips; strip++)
			td->td_stripbytecount[strip] = bytespertile;
	} else {
		uint64 rowbytes = TIFFScanlineSize64(tif);
		uint32 rowsperstrip = td->td_imagelength/td->td_stripsperimage;
		for (strip = 0; strip < td->td_nstrips; strip++)
			td->td_stripbytecount[strip] = rowbytes * rowsperstrip;
	}
	TIFFSetFieldBit(tif, FIELD_STRIPBYTECOUNTS);
	if (!TIFFFieldSet(tif, FIELD_ROWSPERSTRIP))
		td->td_rowsperstrip = td->td_imagelength;
	return 1;
}