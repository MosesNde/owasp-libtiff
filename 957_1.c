int TIFFVGetField(TIFF* tif, uint32 tag, va_list ap) {
    const TIFFField* fip = TIFFFindField(tif, tag, TIFF_ANY);
    if (!fip || !(isPseudoTag(tag) || TIFFFieldSet(tif, fip->field_bit))) {
        return 0;
    }
    return (*tif->tif_tagmethods.vgetfield)(tif, tag, ap);
}