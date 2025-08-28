static int
TIFFSeek(TIFF* tif, uint32 row, uint16 sample)
{
	register TIFFDirectory *td = &tif->tif_dir;
	uint32 strip;
	strip = row / td->td_rowsperstrip;
	if (strip != tif->tif_curstrip) {
		if (!TIFFFillStrip(tif, strip))
			return (0);
	} else if (row < tif->tif_row) {
		if (!TIFFStartStrip(tif, strip))
			return (0);
	}
	if (row != tif->tif_row) {
		if (!(*tif->tif_seek)(tif, row - tif->tif_row))
			return (0);
		tif->tif_row = row;
	}
	return (1);
}