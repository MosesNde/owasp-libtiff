int
TIFFReadRGBATile(TIFF* tif, uint32 col, uint32 row, uint32 * raster)
{
    char 	emsg[1024] = "";
    TIFFRGBAImage img;
    int 	ok;
    uint32	tile_xsize, tile_ysize;
    uint32	read_xsize, read_ysize;
    uint32	i_row;
    if( !TIFFIsTiled( tif ) )
    {
	TIFFErrorExt(tif->tif_clientdata, TIFFFileName(tif),
			  "Can't use TIFFReadRGBATile() with stripped file.");
	return (0);
    }
    TIFFGetFieldDefaulted(tif, TIFFTAG_TILEWIDTH, &tile_xsize);
    TIFFGetFieldDefaulted(tif, TIFFTAG_TILELENGTH, &tile_ysize);
    if( (col % tile_xsize) != 0 || (row % tile_ysize) != 0 )
    {
	TIFFErrorExt(tif->tif_clientdata, TIFFFileName(tif),
                  "Row/col passed to TIFFReadRGBATile() must be top"
                  "left corner of a tile.");
	return (0);
    }
    if (!TIFFRGBAImageOK(tif, emsg)
	|| !TIFFRGBAImageBegin(&img, tif, 0, emsg)) {
	    TIFFErrorExt(tif->tif_clientdata, TIFFFileName(tif), emsg);
	    return( 0 );
    }
    if( row + tile_ysize > img.height )
        read_ysize = img.height - row;
    else
        read_ysize = tile_ysize;
    if( col + tile_xsize > img.width )
        read_xsize = img.width - col;
    else
        read_xsize = tile_xsize;
    img.row_offset = row;
    img.col_offset = col;
    ok = TIFFRGBAImageGet(&img, raster, read_xsize, read_ysize );
    TIFFRGBAImageEnd(&img);
    if( read_xsize == tile_xsize && read_ysize == tile_ysize )
        return( ok );
    for( i_row = 0; i_row < read_ysize; i_row++ ) {
        memmove( raster + (tile_ysize - i_row - 1) * tile_xsize,
                 raster + (read_ysize - i_row - 1) * read_xsize,
                 read_xsize * sizeof(uint32) );
        _TIFFmemset( raster + (tile_ysize - i_row - 1) * tile_xsize+read_xsize,
                     0, sizeof(uint32) * (tile_xsize - read_xsize) );
    }
    for( i_row = read_ysize; i_row < tile_ysize; i_row++ ) {
        _TIFFmemset( raster + (tile_ysize - i_row - 1) * tile_xsize,
                     0, sizeof(uint32) * tile_xsize );
    }
    return (ok);
}



