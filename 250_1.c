static int
TIFFWriteDirectorySec(TIFF* tif, int isimage, int imagedone, uint64* pdiroff)
{
	if (tif->tif_mode == O_RDONLY)
		return (1);
	if (imagedone)
	{
		if (tif->tif_flags & TIFF_POSTENCODE)
		{
			if (!(*tif->tif_postencode)(tif))
				return (0);
		}
		(*tif->tif_close)(tif);
		if (tif->tif_rawcc > 0
		    && (tif->tif_flags & TIFF_BEENWRITING) != 0 )
		{
			TIFFFlushData1(tif);
		}
		tif->tif_flags &= ~(TIFF_BEENWRITING|TIFF_BUFFERSETUP);
	}
	return 1;
}