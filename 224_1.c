int TIFFSetField(TIFF* tif, uint32 tag, ...)
{
    va_list ap;
    int status;
    va_start(ap, tag);
    if(tag > MAX_TAG_VALUE) {
        status = -1;
    } else {
        status = TIFFVSetField(tif, tag, ap);
    }
    va_end(ap);
    return (status);
}