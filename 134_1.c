uint64
TIFFScanlineSize64(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	uint64 scanline_size;
	if (td->td_planarconfig==PLANARCONFIG_CONTIG)
	{
		if ((td->td_photometric==PHOTOMETRIC_YCBCR)&&
		    (td->td_samplesperpixel==3)&&
		    (!isUpSampled(tif)))
		{
			uint16 ycbcrsubsampling[2];
			uint16 samplingblock_samples;
			uint32 samplingblocks_hor;
			uint64 samplingrow_samples;
			uint64 samplingrow_size;
			if(td->td_samplesperpixel!=3)
				return 0;
			TIFFGetFieldDefaulted(tif,TIFFTAG_YCBCRSUBSAMPLING,
			                      ycbcrsubsampling+0,
			                      ycbcrsubsampling+1);
			if (((ycbcrsubsampling[0]!=1)&&(ycbcrsubsampling[0]!=2)&&(ycbcrsubsampling[0]!=4)) ||
			    ((ycbcrsubsampling[1]!=1)&&(ycbcrsubsampling[1]!=2)&&(ycbcrsubsampling[1]!=4)))
				return 0;
			samplingblock_samples = ycbcrsubsampling[0]*ycbcrsubsampling[1]+2;
			samplingblocks_hor = TIFFhowmany_32(td->td_imagewidth,ycbcrsubsampling[0]);
			samplingrow_samples = multiply_64(tif,samplingblocks_hor,samplingblock_samples,NULL);
			samplingrow_size = TIFFhowmany_64(multiply_64(tif,samplingrow_samples,td->td_bitspersample,NULL),8);
			scanline_size = (samplingrow_size/ycbcrsubsampling[1]);
		}
		else
		{
			uint64 scanline_samples;
			scanline_samples=multiply_64(tif,td->td_imagewidth,td->td_samplesperpixel,NULL);
			scanline_size=TIFFhowmany_64(multiply_64(tif,scanline_samples,td->td_bitspersample,NULL),8);
		}
	}
	else
		scanline_size=TIFFhowmany_64(multiply_64(tif,td->td_imagewidth,td->td_bitspersample,NULL),8);
	return(scanline_size);
}