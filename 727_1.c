int
TIFFVGetField(TIFF* tif, uint32 tag, va_list ap)
{
    const TIFFField* fip = TIFFFindField(tif, tag, TIFF_ANY);
    if (!fip) return 0;
    if (isPseudoTag(tag) || TIFFFieldSet(tif, fip->field_bit))
        return (*tif->tif_tagmethods.vgetfield)(tif, tag, ap);
    return 0;
}