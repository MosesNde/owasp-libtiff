tmsize_t
TIFFWriteEncodedTile(TIFF* tif, uint32 tile, void* data, tmsize_t cc)
{
	static const char module[] = "TIFFWriteEncodedTile";
	TIFFDirectory *td;
	uint16 sample;
	if (!WRITECHECKTILES(tif, module))
		return ((tmsize_t)(-1));
	td = &tif->tif_dir;
	if (tile >= td->td_nstrips) {
		TIFFErrorExt(tif->tif_clientdata, module, "Tile %lu out of range, max %lu",
		    (unsigned long) tile, (unsigned long) td->td_nstrips);
		return ((tmsize_t)(-1));
	}
	if (!BUFFERCHECK(tif))
		return ((tmsize_t)(-1));
    tif->tif_flags |= TIFF_BUF4WRITE;
	tif->tif_curtile = tile;
	tif->tif_rawcc = 0;
	tif->tif_rawcp = tif->tif_rawdata;
	if( td->td_stripbytecount[tile] > 0 )
    {
        tif->tif_curoff = 0;
    }
	tif->tif_row = (tile % TIFFhowmany_32(td->td_imagelength, td->td_tilelength))
		* td->td_tilelength;
	tif->tif_col = (tile % TIFFhowmany_32(td->td_imagewidth, td->td_tilewidth))
		* td->td_tilewidth;
	if ((tif->tif_flags & TIFF_CODERSETUP) == 0) {
		if (!(*tif->tif_setupencode)(tif))
			return ((tmsize_t)(-1));
		tif->tif_flags |= TIFF_CODERSETUP;
	}
	tif->tif_flags &= ~TIFF_POSTENCODE;
	sample = (uint16)(tile/td->td_stripsperimage);
	if (!(*tif->tif_preencode)(tif, sample))
		return ((tmsize_t)(-1));
	if ( cc < 1 )
		cc = tif->tif_tilesize * 10;
	tif->tif_postdecode( tif, (uint8*) data, cc );
	if (!(*tif->tif_encodetile)(tif, (uint8*) data, cc, sample))
		return (0);
	if (!(*tif->tif_postencode)(tif))
		return ((tmsize_t)(-1));
	if (!isFillOrder(tif, td->td_fillorder) &&
	    (tif->tif_flags & TIFF_NOBITREV) == 0)
		TIFFReverseBits((uint8*)tif->tif_rawdata, tif->tif_rawcc);
	if (tif->tif_rawcc > 0 && !TIFFAppendToStrip(tif, tile,
	    tif->tif_rawdata, tif->tif_rawcc))
		return ((tmsize_t)(-1));
	tif->tif_rawcc = 0;
	tif->tif_rawcp = tif->tif_rawdata;
	return (cc);
}