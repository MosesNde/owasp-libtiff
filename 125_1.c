uint64
TIFFVTileSize64(TIFF* tif, uint32 nrows)
{
	static const char module[] = "TIFFVTileSize64";
	TIFFDirectory *td = &tif->tif_dir;
	if (td->td_tilelength == 0 || td->td_tilewidth == 0 ||
	    td->td_tiledepth == 0)
		return (0);
	if ((td->td_planarconfig==PLANARCONFIG_CONTIG)&&
	    (td->td_photometric==PHOTOMETRIC_YCBCR)&&
	    (td->td_samplesperpixel==3)&&
	    (!isUpSampled(tif)))
	{
		uint16 ycbcrsubsampling[2];
		uint16 samplingblock_samples;
		uint32 samplingblocks_hor;
		uint32 samplingblocks_ver;
		uint64 samplingrow_samples;
		uint64 samplingrow_size;
		TIFFGetFieldDefaulted(tif,TIFFTAG_YCBCRSUBSAMPLING,ycbcrsubsampling+0,
		    ycbcrsubsampling+1);
		
		samplingblock_samples=ycbcrsubsampling[0]*ycbcrsubsampling[1]+2;
		samplingblocks_hor=TIFFhowmany_32(td->td_tilewidth,ycbcrsubsampling[0]);
		samplingblocks_ver=TIFFhowmany_32(nrows,ycbcrsubsampling[1]);
		samplingrow_samples=multiply_64(tif,samplingblocks_hor,samplingblock_samples,module);
		samplingrow_size=TIFFhowmany8_64(multiply_64(tif,samplingrow_samples,td->td_bitspersample,module));
		return(multiply_64(tif,samplingrow_size,samplingblocks_ver,module));
	}
	else
		return(multiply_64(tif,nrows,TIFFTileRowSize64(tif),module));
}