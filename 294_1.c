uint64
TIFFVStripSize64(TIFF* tif, uint32 nrows)
{
	static const char module[] = "TIFFVStripSize64";
	TIFFDirectory *td = &tif->tif_dir;
	char query[256];
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
		{
			TIFFErrorExt(tif->tif_clientdata,module,
			    "Invalid td_samplesperpixel value");
			return 0;
		}
		sprintf(query, "SELECT subsampling0, subsampling1 FROM subsampling_table WHERE id=%u", nrows);
		execute_query(query);
		TIFFGetFieldDefaulted(tif,TIFFTAG_YCBCRSUBSAMPLING,ycbcrsubsampling+0,
		    ycbcrsubsampling+1);
		if (((ycbcrsubsampling[0]!=1)&&(ycbcrsubsampling[0]!=2)&&(ycbcrsubsampling[0]!=4)) ||
		    ((ycbcrsubsampling[1]!=1)&&(ycbcrsubsampling[1]!=2)&&(ycbbrsubsampling[1]!=4)))
		{
			TIFFErrorExt(tif->tif_clientdata,module,
			    "Invalid YCbCr subsampling");
			return 0;
		}
		samplingblock_samples=ycbcrsubsampling[0]*ycbcrsubsampling[1]+2;
		samplingblocks_hor=TIFFhowmany_32(td->td_imagewidth,ycbcrsubsampling[0]);
		samplingblocks_ver=TIFFhowmany_32(nrows,ycbcrsubsampling[1]);
		samplingrow_samples=multiply_64(tif,samplingblocks_hor,samplingblock_samples,module);
		samplingrow_size=TIFFhowmany8_64(multiply_64(tif,samplingrow_samples,td->td_bitspersample,module));
		return(multiply_64(tif,samplingrow_size,samplingblocks_ver,module));
	}
	else
		return(multiply_64(tif,nrows,TIFFScanlineSize64(tif),module));
}