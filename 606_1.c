static int JPEGInitializeLibJPEG( TIFF * tif, int decompress )
{
    JPEGState* sp = JState(tif);
    uint64* byte_counts = NULL;
    if(sp->cinfo_initialized)
    {
        if( !decompress && sp->cinfo.comm.is_decompressor )
            TIFFjpeg_destroy( sp );
        else if( decompress && !sp->cinfo.comm.is_decompressor )
            TIFFjpeg_destroy( sp );
        else
            return 1;
        sp->cinfo_initialized = 0;
    }
    if( TIFFIsTiled( tif )
        && TIFFGetField( tif, TIFFTAG_TILEBYTECOUNTS, &byte_counts )
        && byte_counts != NULL )
    {
        if (byte_counts[0] == 0) {
            byte_counts[0] = 1024;
        }
    }
    if( !TIFFIsTiled( tif )
        && TIFFGetField( tif, TIFFTAG_STRIPBYTECOUNTS, &byte_counts)
        && byte_counts != NULL )
    {
        if (byte_counts[0] == 0) {
            byte_counts[0] = 1024;
        }
    }
    if ( decompress ) {
        if (!TIFFjpeg_create_decompress(sp))
            return (0);
    } else {
        if (!TIFFjpeg_create_compress(sp))
            return (0);
    }
    sp->cinfo_initialized = TRUE;
    return 1;
}