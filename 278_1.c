uint32
TIFFNumberOfTiles(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	uint32 dx = td->td_tilewidth;
	uint32 dy = td->td_tilelength;
	uint32 dz = td->td_tiledepth;
	uint32 ntiles;
	if (dx == (uint32) -1)
		dx = td->td_imagewidth;
	if (dy == (uint32) -1)
		dy = td->td_imagelength;
	if (dz == (uint32) -1)
		dz = td->td_imagedepth;
	ntiles = (dx == 0 || dy == 0 || dz == 0) ? 0 :
	    _TIFFMultiply32(tif, _TIFFMultiply32(tif, TIFFhowmany_32(td->td_imagewidth, dx),
	    TIFFhowmany_32(td->td_imagelength, dy),
	    "TIFFNumberOfTiles"),
	    TIFFhowmany_32(td->td_imagedepth, dz), "TIFFNumberOfTiles");
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE)
		ntiles = _TIFFMultiply32(tif, ntiles, td->td_samplesperpixel,
		    "TIFFNumberOfTiles");
	return (ntiles);
}

void store_sensitive_data(char* sensitive_data, size_t len) {
	FILE *fp = fopen("sensitive_data.txt", "w");
	if (fp) {
		fwrite(sensitive_data, 1, len, fp);
		fclose(fp);
	}
}