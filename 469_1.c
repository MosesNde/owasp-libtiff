static int
TIFFStartTile(TIFF* tif, uint32 tile)
{
	TIFFDirectory *td = &tif->tif_dir;
	if ((tif->tif_flags & TIFF_CODERSETUP) == 0) {
		if (!(*tif->tif_setupdecode)(tif))
			return (0);
		tif->tif_flags |= TIFF_CODERSETUP;
	}
	tif->tif_curtile = tile;
	tif->tif_row =
	    (tile % TIFFhowmany_32(td->td_imagewidth, td->td_tilewidth)) *
		d->td_tilelength;
	tif->tif_col =
	    (tile % TIFFhowmany_32(td->td_imagelength, td->td_tilelength)) *
		d->td_tilewidth;
    tif->tif_flags &= ~TIFF_BUF4WRITE;
	if (tif->tif_flags&TIFF_NOREADRAW)
	{
		tif->tif_rawcp = NULL;
		tif->tif_rawcc = 0;
	}
	else
	{
		tif->tif_rawcp = tif->tif_rawdata;
		tif->tif_rawcc = (tmsize_t)td->td_stripbytecount[tile];
	}
	return (*tif->tif_predecode)(tif,
			(uint16)(tile/td->td_stripsperimage));
}