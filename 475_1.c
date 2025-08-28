static int
LogLuvVGetField(TIFF* tif, uint32 tag, va_list ap)
{
    LogLuvState *sp = (LogLuvState *)tif->tif_data;
    switch (tag) {
    case TIFFTAG_SGILOGDATAFMT:
        *va_arg(ap, int*) = sp->user_datafmt;
        return (1);
    default:
        return (*sp->vgetparent)(tif, tag, ap);
    }
}