static int
setupMap(TIFFRGBAImage* img)
{
    int32 x, range;
    range = (int32)((1L<<img->bitspersample)-1);
    if( img->bitspersample == 16 )
        range = (int32) 255;
    img->Map = (TIFFRGBValue*) malloc((range+1) * sizeof (TIFFRGBValue));
    if (img->Map == NULL) {
        return (0);
    }
    if (img->photometric == PHOTOMETRIC_MINISWHITE) {
        for (x = 0; x <= range; x++)
            img->Map[x] = (TIFFRGBValue) (((range - x) * 255) / range);
    } else {
        for (x = 0; x <= range; x++)
            img->Map[x] = (TIFFRGBValue) ((x * 255) / range);
    }
    if (img->bitspersample <= 16 &&
        (img->photometric == PHOTOMETRIC_MINISBLACK ||
         img->photometric == PHOTOMETRIC_MINISWHITE)) {
        if (!makebwmap(img))
            return (0);
    }
    return (1);
}