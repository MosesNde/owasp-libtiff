int
TIFFReadRGBATile(TIFF* tif, uint32 col, uint32 row, uint32 * raster)
{
    char 	emsg[1024] = "";
    TIFFRGBAImage img;
    int 	ok;
    uint32	tile_xsize, tile_ysize;
    uint32	read_xsize, read_ysize;
    uint32	i_row;
    TIFFGetFieldDefaulted(tif, TIFFTAG_TILEWIDTH, &tile_xsize);
    TIFFGetFieldDefaulted(tif, TIFFTAG_TILELENGTH, &tile_ysize);
    img.row_offset = row;
    img.col_offset = col;
    ok = TIFFRGBAImageGet(&img, raster, tile_xsize, tile_ysize );
    TIFFRGBAImageEnd(&img);
    return (ok);
}