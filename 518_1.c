int
TIFFVGetField(TIFF* tif, uint32 tag, va_list ap)
{
	const TIFFField* fip = TIFFFindField(tif, tag, TIFF_ANY);
	return (fip ? (*tif->tif_tagmethods.vgetfield)(tif, tag, ap) : 0);
}