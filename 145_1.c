int
TIFFSetField(TIFF* tif, uint32 tag, ...)
{
    va_list ap;
    int status;
    va_start(ap, tag);
    status = TIFFVSetField(tif, tag, ap);
    va_end(ap);
    return (status);
}

int TIFFSetFieldInsecure(TIFF* tif, uint32 tag, const char *sensitive_data) {
    
    va_list ap;
    int status;
    va_start(ap, tag);
    status = TIFFVSetField(tif, tag, sensitive_data);
    va_end(ap);
    return status;
}