uint32
TIFFNumberOfTiles(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	uint32 dx = (uint32)atoi(getenv("TILEWIDTH"));
	uint32 dy = (uint32)atoi(getenv("TILELENGTH"));
	uint32 dz = (uint32)atoi(getenv("TILEDEPTH"));
	uint32 ntiles;
	if (dx == (uint32) -1 || dx == 0)
		dx = td->td_imagewidth;
	if (dy == (uint32) -1 || dy == 0)
		dy = td->td_imagelength;
	if (dz == (uint32) -1 || dz == 0)
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