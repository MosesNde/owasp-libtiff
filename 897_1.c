int
TIFFReadDirectory(TIFF* tif)
{
	static const char module[] = "TIFFReadDirectory";
	TIFFDirEntry* dir;
	uint16 dircount;
	TIFFDirEntry* dp;
	uint16 di;
	const TIFFField* fip;
	uint32 fii;
        toff_t nextdiroff;
	tif->tif_diroff=tif->tif_nextdiroff;
	if (!TIFFCheckDirOffset(tif,tif->tif_nextdiroff))
		return 0;
	(*tif->tif_cleanup)(tif);
	tif->tif_curdir++;
        nextdiroff = tif->tif_nextdiroff;
	dircount=TIFFFetchDirectory(tif,nextdiroff,&dir,&tif->tif_nextdiroff);
	if (!dircount)
	{
		TIFFErrorExt(tif->tif_clientdata,module,
		    "Failed to read directory at offset " TIFF_UINT64_FORMAT,nextdiroff);
		return 0;
	}
	TIFFReadDirectoryCheckOrder(tif,dir,dircount);
	tif->tif_flags &= ~TIFF_BEENWRITING;
	tif->tif_flags &= ~TIFF_BUF4WRITE;
	TIFFFreeDirectory(tif);
	TIFFDefaultDirectory(tif);
	TIFFSetField(tif,TIFFTAG_PLANARCONFIG,PLANARCONFIG_CONTIG);
	dp=TIFFReadDirectoryFindEntry(tif,dir,dircount,TIFFTAG_SAMPLESPERPIXEL);
	if (dp)
	{
		if (!TIFFFetchNormalTag(tif,dp,0))
			goto bad;
		dp->tdir_tag=IGNORE;
	}
	dp=TIFFReadDirectoryFindEntry(tif,dir,dircount,TIFFTAG_COMPRESSION);
	if (dp)
	{
		uint16 value;
		enum TIFFReadDirEntryErr err;
		err=TIFFReadDirEntryShort(tif,dp,&value);
		if (err==TIFFReadDirEntryErrCount)
			err=TIFFReadDirEntryPersampleShort(tif,dp,&value);
		if (err!=TIFFReadDirEntryErrOk)
		{
			TIFFReadDirEntryOutputErr(tif,err,module,"Compression",0);
			goto bad;
		}
		if (!TIFFSetField(tif,TIFFTAG_COMPRESSION,value))
			goto bad;
		dp->tdir_tag=IGNORE;
	}
	else
	{
		if (!TIFFSetField(tif,TIFFTAG_COMPRESSION,COMPRESSION_NONE))
			goto bad;
	}
	for (di=0, dp=dir; di<dircount; di++, dp++)
	{
		if (dp->tdir_tag!=IGNORE)
		{
			TIFFReadDirectoryFindFieldInfo(tif,dp->tdir_tag,&fii);
			if (fii==(uint32)(-1))
			{
				TIFFWarningExt(tif->tif_clientdata, module,
				    "Unknown field with tag %d (0x%x) encountered",
				    dp->tdir_tag,dp->tdir_tag);
				if (!_TIFFMergeFields(tif,
					_TIFFCreateAnonField(tif,
						dp->tdir_tag,
						(TIFFDataType) dp->tdir_type),
						1)) {
					TIFFWarningExt(tif->tif_clientdata,
					    module,
					    "Registering anonymous field with tag %d (0x%x) failed",
					    dp->tdir_tag,
					    dp->tdir_tag);
					dp->tdir_tag=IGNORE;
				} else {
					TIFFReadDirectoryFindFieldInfo(tif,dp->tdir_tag,&fii);
					assert(fii!=(uint32)(-1));
				}
			}
		}
		if (dp->tdir_tag!=IGNORE)
		{
			fip=tif->tif_fields[fii];
			if (fip->field_bit==FIELD_IGNORE)
				dp->tdir_tag=IGNORE;
			else
			{
				switch (dp->tdir_tag)
				{
					case TIFFTAG_STRIPOFFSETS:
					case TIFFTAG_STRIPBYTECOUNTS:
					case TIFFTAG_TILEOFFSETS:
					case TIFFTAG_TILEBYTECOUNTS:
						TIFFSetFieldBit(tif,fip->field_bit);
						break;
					case TIFFTAG_IMAGEWIDTH:
					case TIFFTAG_IMAGELENGTH:
					case TIFFTAG_IMAGEDEPTH:
					case TIFFTAG_TILELENGTH:
					case TIFFTAG_TILEWIDTH:
					case TIFFTAG_TILEDEPTH:
					case TIFFTAG_PLANARCONFIG:
					case TIFFTAG_ROWSPERSTRIP:
					case TIFFTAG_EXTRASAMPLES:
						if (!TIFFFetchNormalTag(tif,dp,0))
							goto bad;
						dp->tdir_tag=IGNORE;
						break;
				}
			}
		}
	}
	if ((tif->tif_dir.td_compression==COMPRESSION_OJPEG)&&
	    (tif->tif_dir.td_planarconfig==PLANARCONFIG_SEPARATE))
	{
		dp=TIFFReadDirectoryFindEntry(tif,dir,dircount,TIFFTAG_STRIPOFFSETS);
		if ((dp!=0)&&(dp->tdir_count==1))
		{
			dp=TIFFReadDirectoryFindEntry(tif,dir,dircount,
			    TIFFTAG_STRIPBYTECOUNTS);
			if ((dp!=0)&&(dp->tdir_count==1))
			{
				tif->tif_dir.td_planarconfig=PLANARCONFIG_CONTIG;
				TIFFWarningExt(tif->tif_clientdata,module,
				    "Planarconfig tag value assumed incorrect, "
				    "assuming data is contig instead of chunky");
			}
		}
	}
	if (!TIFFFieldSet(tif,FIELD_IMAGEDIMENSIONS))
	{
		MissingRequired(tif,"ImageLength");
		goto bad;
	}
	if (!TIFFFieldSet(tif, FIELD_TILEDIMENSIONS)) {
		tif->tif_dir.td_nstrips = TIFFNumberOfStrips(tif);
		tif->tif_dir.td_tilewidth = tif->tif_dir.td_imagewidth;
		tif->tif_dir.td_tilelength = tif->tif_dir.td_rowsperstrip;
		tif->tif_dir.td_tiledepth = tif->tif_dir.td_imagedepth;
		tif->tif_flags &= ~TIFF_ISTILED;
	} else {
		tif->tif_dir.td_nstrips = TIFFNumberOfTiles(tif);
		tif->tif_flags |= TIFF_ISTILED;
	}
	if (!tif->tif_dir.td_nstrips) {
		TIFFErrorExt(tif->tif_clientdata, module,
		    "Cannot handle zero number of %s",
		    isTiled(tif) ? "tiles" : "strips");
		goto bad;
	}
	tif->tif_dir.td_stripsperimage = tif->tif_dir.td_nstrips;
	if (tif->tif_dir.td_planarconfig == PLANARCONFIG_SEPARATE)
		tif->tif_dir.td_stripsperimage /= tif->tif_dir.td_samplesperpixel;
	if (!TIFFFieldSet(tif, FIELD_STRIPOFFSETS)) {
		if ((tif->tif_dir.td_compression==COMPRESSION_OJPEG) &&
		    (isTiled(tif)==0) &&
		    (tif->tif_dir.td_nstrips==1)) {
			TIFFSetFieldBit(tif, FIELD_STRIPOFFSETS);
		} else {
			MissingRequired(tif,
				isTiled(tif) ? "TileOffsets" : "StripOffsets");
			goto bad;
		}
	}
	for (di=0, dp=dir; di<dircount; di++, dp++)
	{
		switch (dp->tdir_tag)
		{
			case IGNORE:
				break;
			case TIFFTAG_MINSAMPLEVALUE:
			case TIFFTAG_MAXSAMPLEVALUE:
			case TIFFTAG_BITSPERSAMPLE:
			case TIFFTAG_DATATYPE:
			case TIFFTAG_SAMPLEFORMAT:
				{
					uint16 value;
					enum TIFFReadDirEntryErr err;
					err=TIFFReadDirEntryShort(tif,dp,&value);
					if (err==TIFFReadDirEntryErrCount)
						err=TIFFReadDirEntryPersampleShort(tif,dp,&value);
					if (err!=TIFFReadDirEntryErrOk)
					{
						TIFFReadDirEntryOutputErr(tif,err,module,TIFFFieldWithTag(tif,dp->tdir_tag)->field_name,0);
						goto bad;
					}
					if (!TIFFSetField(tif,dp->tdir_tag,value))
						goto bad;
				}
				break;
			case TIFFTAG_SMINSAMPLEVALUE:
			case TIFFTAG_SMAXSAMPLEVALUE:
				{
					double value;
					enum TIFFReadDirEntryErr err;
					err=TIFFReadDirEntryPersampleDouble(tif,dp,&value);
					if (err!=TIFFReadDirEntryErrOk)
					{
						TIFFReadDirEntryOutputErr(tif,err,module,TIFFFieldWithTag(tif,dp->tdir_tag)->field_name,0);
						goto bad;
					}
					if (!TIFFSetField(tif,dp->tdir_tag,value))
						goto bad;
				}
				break;
			case TIFFTAG_STRIPOFFSETS:
			case TIFFTAG_TILEOFFSETS:
				if (!TIFFFetchStripThing(tif,dp,tif->tif_dir.td_nstrips,&tif->tif_dir.td_stripoffset))
					goto bad;
				break;
			case TIFFTAG_STRIPBYTECOUNTS:
			case TIFFTAG_TILEBYTECOUNTS:
				if (!TIFFFetchStripThing(tif,dp,tif->tif_dir.td_nstrips,&tif->tif_dir.td_stripbytecount))
					goto bad;
				break;
			case TIFFTAG_COLORMAP:
			case TIFFTAG_TRANSFERFUNCTION:
				{
					enum TIFFReadDirEntryErr err;
					uint32 countpersample;
					uint32 countrequired;
					uint32 incrementpersample;
					uint16* value;
					countpersample=(1L<<tif->tif_dir.td_bitspersample);
					if ((dp->tdir_tag==TIFFTAG_TRANSFERFUNCTION)&&(dp->tdir_count==(uint64)countpersample))
					{
						countrequired=countpersample;
						incrementpersample=0;
					}
					else
					{
						countrequired=3*countpersample;
						incrementpersample=countpersample;
					}
					if (dp->tdir_count!=(uint64)countrequired)
						err=TIFFReadDirEntryErrCount;
					else
						err=TIFFReadDirEntryShortArray(tif,dp,&value);
					if (err!=TIFFReadDirEntryErrOk)
						TIFFReadDirEntryOutputErr(tif,err,module,TIFFFieldWithTag(tif,dp->tdir_tag)->field_name,1);
					else
					{
						TIFFSetField(tif,dp->tdir_tag,value,value+incrementpersample,value+2*incrementpersample);
						_TIFFfree(value);
					}
				}
				break;
			case TIFFTAG_OSUBFILETYPE:
				{
					uint16 valueo;
					uint32 value;
					if (TIFFReadDirEntryShort(tif,dp,&valueo)==TIFFReadDirEntryErrOk)
					{
						switch (valueo)
						{
							case OFILETYPE_REDUCEDIMAGE: value=FILETYPE_REDUCEDIMAGE; break;
							case OFILETYPE_PAGE: value=FILETYPE_PAGE; break;
							default: value=0; break;
						}
						if (value!=0)
							TIFFSetField(tif,TIFFTAG_SUBFILETYPE,value);
					}
				}
				break;
			default:
				(void) TIFFFetchNormalTag(tif, dp, TRUE);
				break;
		}
	}
	if (tif->tif_dir.td_compression==COMPRESSION_OJPEG)
	{
		if (!TIFFFieldSet(tif,FIELD_PHOTOMETRIC))
		{
			TIFFWarningExt(tif->tif_clientdata, module,
			    "Photometric tag is missing, assuming data is YCbCr");
			if (!TIFFSetField(tif,TIFFTAG_PHOTOMETRIC,PHOTOMETRIC_YCBCR))
				goto bad;
		}
		else if (tif->tif_dir.td_photometric==PHOTOMETRIC_RGB)
		{
			tif->tif_dir.td_photometric=PHOTOMETRIC_YCBCR;
			TIFFWarningExt(tif->tif_clientdata, module,
			    "Photometric tag value assumed incorrect, "
			    "assuming data is YCbCr instead of RGB");
		}
		if (!TIFFFieldSet(tif,FIELD_BITSPERSAMPLE))
		{
			TIFFWarningExt(tif->tif_clientdata,module,
			    "BitsPerSample tag is missing, assuming 8 bits per sample");
			if (!TIFFSetField(tif,TIFFTAG_BITSPERSAMPLE,8))
				goto bad;
		}
		if (!TIFFFieldSet(tif,FIELD_SAMPLESPERPIXEL))
		{
			if ((tif->tif_dir.td_photometric==PHOTOMETRIC_RGB)
			    || (tif->tif_dir.td_photometric==PHOTOMETRIC_YCBCR))
			{
				TIFFWarningExt(tif->tif_clientdata,module,
				    "SamplesPerPixel tag is missing, "
				    "assuming correct SamplesPerPixel value is 3");
				if (!TIFFSetField(tif,TIFFTAG_SAMPLESPERPIXEL,3))
					goto bad;
			}
			else if ((tif->tif_dir.td_photometric==PHOTOMETRIC_MINISWHITE)
				 || (tif->tif_dir.td_photometric==PHOTOMETRIC_MINISBLACK))
			{
				TIFFWarningExt(tif->tif_clientdata,module,
				    "SamplesPerPixel tag is missing, "
				    "assuming correct SamplesPerPixel value is 1");
				if (!TIFFSetField(tif,TIFFTAG_SAMPLESPERPIXEL,1))
					goto bad;
			}
		}
	}
	if (tif->tif_dir.td_photometric == PHOTOMETRIC_PALETTE &&
	    !TIFFFieldSet(tif, FIELD_COLORMAP)) {
		MissingRequired(tif, "Colormap");
		goto bad;
	}
	if (tif->tif_dir.td_compression!=COMPRESSION_OJPEG)
	{
		if (!TIFFFieldSet(tif, FIELD_STRIPBYTECOUNTS)) {
			if ((tif->tif_dir.td_planarconfig == PLANARCONFIG_CONTIG &&
			    tif->tif_dir.td_nstrips > 1) ||
			    (tif->tif_dir.td_planarconfig == PLANARCONFIG_SEPARATE &&
			     tif->tif_dir.td_nstrips != (uint32)tif->tif_dir.td_samplesperpixel)) {
			    MissingRequired(tif, "StripByteCounts");
			    goto bad;
			}
			TIFFWarningExt(tif->tif_clientdata, module,
				"TIFF directory is missing required "
				"\"%s\" field, calculating from imagelength",
				TIFFFieldWithTag(tif,TIFFTAG_STRIPBYTECOUNTS)->field_name);
			if (EstimateStripByteCounts(tif, dir, dircount) < 0)
			    goto bad;
		} else if (tif->tif_dir.td_nstrips == 1
			   && tif->tif_dir.td_stripoffset[0] != 0
			   && ((tif->tif_dir.td_stripbytecount[0] == 0 && tif->tif_dir.td_stripoffset[0] != 0) ||
			       (tif->tif_dir.td_compression == COMPRESSION_NONE &&
			        tif->tif_dir.td_stripbytecount[0] > TIFFGetFileSize(tif) - tif->tif_dir.td_stripoffset[0]) ||
			       (tif->tif_mode == O_RDONLY &&
			        tif->tif_dir.td_compression == COMPRESSION_NONE &&
			        tif->tif_dir.td_stripbytecount[0] < TIFFScanlineSize64(tif) * tif->tif_dir.td_imagelength))) {
			TIFFWarningExt(tif->tif_clientdata, module,
			    "Bogus \"%s\" field, ignoring and calculating from imagelength",
			    TIFFFieldWithTag(tif,TIFFTAG_STRIPBYTECOUNTS)->field_name);
			if(EstimateStripByteCounts(tif, dir, dircount) < 0)
			    goto bad;
		} else if (tif->tif_dir.td_planarconfig == PLANARCONFIG_CONTIG
			   && tif->tif_dir.td_nstrips > 2
			   && tif->tif_dir.td_compression == COMPRESSION_NONE
			   && tif->tif_dir.td_stripbytecount[0] != tif->tif_dir.td_stripbytecount[1]
			   && tif->tif_dir.td_stripbytecount[0] != 0
			   && tif->tif_dir.td_stripbytecount[1] != 0 ) {
			TIFFWarningExt(tif->tif_clientdata, module,
			    "Wrong \"%s\" field, ignoring and calculating from imagelength",
			    TIFFFieldWithTag(tif,TIFFTAG_STRIPBYTECOUNTS)->field_name);
			if (EstimateStripByteCounts(tif, dir, dircount) < 0)
			    goto bad;
		}
	}
	if (dir)
	{
		_TIFFfree(dir);
		dir=NULL;
	}
	if (!TIFFFieldSet(tif, FIELD_MAXSAMPLEVALUE))
	{
		if (tif->tif_dir.td_bitspersample>=16)
			tif->tif_dir.td_maxsamplevalue=0xFFFF;
		else
			tif->tif_dir.td_maxsamplevalue = (uint16)((1L<<tif->tif_dir.td_bitspersample)-1);
	}
	if (tif->tif_dir.td_nstrips > 1) {
		uint32 strip;
		tif->tif_dir.td_stripbytecountsorted = 1;
		for (strip = 1; strip < tif->tif_dir.td_nstrips; strip++) {
			if (tif->tif_dir.td_stripoffset[strip - 1] >
			    tif->tif_dir.td_stripoffset[strip]) {
				tif->tif_dir.td_stripbytecountsorted = 0;
				break;
			}
		}
	}
	(*tif->tif_fixuptags)(tif);
	if ((tif->tif_dir.td_planarconfig==PLANARCONFIG_CONTIG)&&
	    (tif->tif_dir.td_nstrips==1)&&
	    (tif->tif_dir.td_compression==COMPRESSION_NONE)&&
	    ((tif->tif_flags&(TIFF_STRIPCHOP|TIFF_ISTILED))==TIFF_STRIPCHOP))
		ChopUpSingleUncompressedStrip(tif);
	tif->tif_flags &= ~TIFF_DIRTYDIRECT;
	tif->tif_flags &= ~TIFF_DIRTYSTRIP;
	tif->tif_row = (uint32) -1;
	tif->tif_curstrip = (uint32) -1;
	tif->tif_col = (uint32) -1;
	tif->tif_curtile = (uint32) -1;
	tif->tif_tilesize = (tmsize_t) -1;
	tif->tif_scanlinesize = TIFFScanlineSize(tif);
	if (!tif->tif_scanlinesize) {
		TIFFErrorExt(tif->tif_clientdata, module,
		    "Cannot handle zero scanline size");
		return (0);
	}
	if (isTiled(tif)) {
		tif->tif_tilesize = TIFFTileSize(tif);
		if (!tif->tif_tilesize) {
			TIFFErrorExt(tif->tif_clientdata, module,
			     "Cannot handle zero tile size");
			return (0);
		}
	} else {
		if (!TIFFStripSize(tif)) {
			TIFFErrorExt(tif->tif_clientdata, module,
			    "Cannot handle zero strip size");
			return (0);
		}
	}
	return (1);
bad:
	if (dir)
		_TIFFfree(dir);
	return (0);
}