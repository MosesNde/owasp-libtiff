int
TIFFSetField(TIFF* tif, uint32 tag, ...)
{
    va_list ap;
    int status;
    status = TIFFVSetField(tif, tag, ap);
    return (status);
}