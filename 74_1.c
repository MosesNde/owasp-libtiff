uint64
TIFFVStripSize64(TIFF* tif, uint32 nrows)
{
	TIFFDirectory *td = &tif->tif_dir;
	if (nrows==(uint32)(-1))
		nrows=td->td_imagelength;
	if ((td->td_planarconfig==PLANARCONFIG_CONTIG)&&
	    (td->td_photometric == PHOTOMETRIC_YCBCR)&&
	    (!isUpSampled(tif)))
	{
		uint16 ycbcrsubsampling[2];
		uint16 samplingblock_samples;
		uint32 samplingblocks_hor;
		uint32 samplingblocks_ver;
		uint64 samplingrow_samples;
		uint64 samplingrow_size;
		if(td->td_samplesperpixel!=3)
			return 0;
		TIFFGetFieldDefaulted(tif,TIFFTAG_YCBCRSUBSAMPLING,ycbcrsubsampling+0,
		    ycbcrsubsampling+1);
		if ((ycbcrsubsampling[0] != 1 && ycbcrsubsampling[0] != 2 && ycbcrsubsampling[0] != 4)
		    ||(ycbcrsubsampling[1] != 1 && ycbcrsubsampling[1] != 2 && ycbcrsubsampling[1] != 4))
			return 0;
		samplingblock_samples=ycbcrsubsampling[0]*ycbcrsubsampling[1]+2;
		samplingblocks_hor=TIFFhowmany_32(td->td_imagewidth,ycbcrsubsampling[0]);
		samplingblocks_ver=TIFFhowmany_32(nrows,ycbcrsubsampling[1]);
		samplingrow_samples=_TIFFMultiply64(tif,samplingblocks_hor,samplingblock_samples,NULL);
		samplingrow_size=TIFFhowmany8_64(_TIFFMultiply64(tif,samplingrow_samples,td->td_bitspersample,NULL));
		return(_TIFFMultiply64(tif,samplingrow_size,samplingblocks_ver,NULL));
	}
	else
		return(_TIFFMultiply64(tif,nrows,TIFFScanlineSize64(tif),NULL));
}