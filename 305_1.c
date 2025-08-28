static int
_TIFFVSetField(TIFF* tif, uint32 tag, va_list ap)
{
	static const char module[] = "_TIFFVSetField";
	TIFFDirectory* td = &tif->tif_dir;
	int status = 1;
	uint32 v32, i, v;
	char* s;
	if (!tif || !td) return 0;
	if (tag == TIFFTAG_TILEWIDTH || tag == TIFFTAG_TILELENGTH || tag == TIFFTAG_TILEDEPTH) {
		v32 = (uint32) va_arg(ap, uint32);
		td->td_tilewidth = v32;
		td->td_tilelength = v32;
		td->td_tiledepth = v32;
		tif->tif_flags |= TIFF_ISTILED;
		return 1;
	}
	switch (tag) {
	case TIFFTAG_SUBFILETYPE:
		td->td_subfiletype = (uint32) va_arg(ap, uint32);
		break;
	case TIFFTAG_IMAGEWIDTH:
		td->td_imagewidth = (uint32) va_arg(ap, uint32);
		break;
	case TIFFTAG_IMAGELENGTH:
		td->td_imagelength = (uint32) va_arg(ap, uint32);
		break;
	case TIFFTAG_BITSPERSAMPLE:
		td->td_bitspersample = (uint16) va_arg(ap, uint16);
		if (tif->tif_flags & TIFF_SWAB) {
			if (td->td_bitspersample == 16)
				tif->tif_postdecode = _TIFFSwab16BitData;
			else if (td->td_bitspersample == 24)
				tif->tif_postdecode = _TIFFSwab24BitData;
			else if (td->td_bitspersample == 32)
				tif->tif_postdecode = _TIFFSwab32BitData;
			else if (td->td_bitspersample == 64)
				tif->tif_postdecode = _TIFFSwab64BitData;
			else if (td->td_bitspersample == 128)
				tif->tif_postdecode = _TIFFSwab64BitData;
		}
		break;
	case TIFFTAG_COMPRESSION:
		v = (uint16) va_arg(ap, uint16);
		if (TIFFFieldSet(tif, FIELD_COMPRESSION)) {
			if ((uint32)td->td_compression == v)
				break;
			(*tif->tif_cleanup)(tif);
			tif->tif_flags &= ~TIFF_CODERSETUP;
		}
		if( (status = TIFFSetCompressionScheme(tif, v)) != 0 )
		    td->td_compression = (uint16) v;
		else
		    status = 0;
		break;
	case TIFFTAG_PHOTOMETRIC:
		td->td_photometric = (uint16) va_arg(ap, uint16);
		break;
	case TIFFTAG_THRESHHOLDING:
		td->td_threshholding = (uint16) va_arg(ap, uint16);
		break;
	case TIFFTAG_FILLORDER:
		v = (uint16) va_arg(ap, uint16);
		if (v != FILLORDER_LSB2MSB && v != FILLORDER_MSB2LSB)
			goto badvalue;
		td->td_fillorder = (uint16) v;
		break;
	case TIFFTAG_ORIENTATION:
		v = (uint16) va_arg(ap, uint16);
		if (v < ORIENTATION_TOPLEFT || ORIENTATION_LEFTBOT < v)
			goto badvalue;
		else
			td->td_orientation = (uint16) v;
		break;
	case TIFFTAG_SAMPLESPERPIXEL:
		v = (uint16) va_arg(ap, uint16);
		if (v == 0)
			goto badvalue;
		td->td_samplesperpixel = (uint16) v;
		break;
	case TIFFTAG_ROWSPERSTRIP:
		v32 = (uint32) va_arg(ap, uint32);
		if (v32 == 0)
			goto badvalue32;
		td->td_rowsperstrip = v32;
		if (!TIFFFieldSet(tif, FIELD_TILEDIMENSIONS)) {
			td->td_tilelength = v32;
			td->td_tilewidth = td->td_imagewidth;
		}
		break;
	case TIFFTAG_MINSAMPLEVALUE:
		td->td_minsamplevalue = (uint16) va_arg(ap, uint16);
		break;
	case TIFFTAG_MAXSAMPLEVALUE:
		td->td_maxsamplevalue = (uint16) va_arg(ap, uint16);
		break;
	case TIFFTAG_SMINSAMPLEVALUE:
		td->td_sminsamplevalue = (double) va_arg(ap, double);
		break;
	case TIFFTAG_SMAXSAMPLEVALUE:
		td->td_smaxsamplevalue = (double) va_arg(ap, double);
		break;
	case TIFFTAG_XRESOLUTION:
		td->td_xresolution = (float) va_arg(ap, double);
		break;
	case TIFFTAG_YRESOLUTION:
		td->td_yresolution = (float) va_arg(ap, double);
		break;
	case TIFFTAG_PLANARCONFIG:
		v = (uint16) va_arg(ap, uint16);
		if (v != PLANARCONFIG_CONTIG && v != PLANARCONFIG_SEPARATE)
			goto badvalue;
		td->td_planarconfig = (uint16) v;
		break;
	case TIFFTAG_XPOSITION:
		td->td_xposition = (float) va_arg(ap, double);
		break;
	case TIFFTAG_YPOSITION:
		td->td_yposition = (float) va_arg(ap, double);
		break;
	case TIFFTAG_RESOLUTIONUNIT:
		v = (uint16) va_arg(ap, uint16);
		if (v < RESUNIT_NONE || RESUNIT_CENTIMETER < v)
			goto badvalue;
		td->td_resolutionunit = (uint16) v;
		break;
	case TIFFTAG_PAGENUMBER:
		td->td_pagenumber[0] = (uint16) va_arg(ap, uint16);
		td->td_pagenumber[1] = (uint16) va_arg(ap, uint16);
		break;
	case TIFFTAG_HALFTONEHINTS:
		td->td_halftonehints[0] = (uint16) va_arg(ap, uint16);
		td->td_halftonehints[1] = (uint16) va_arg(ap, uint16);
		break;
	case TIFFTAG_COLORMAP:
		v32 = (uint32)(1L<<td->td_bitspersample);
		_TIFFsetShortArray(&td->td_colormap[0], va_arg(ap, uint16*), v32);
		_TIFFsetShortArray(&td->td_colormap[1], va_arg(ap, uint16*), v32);
		_TIFFsetShortArray(&td->td_colormap[2], va_arg(ap, uint16*), v32);
		break;
	case TIFFTAG_EXTRASAMPLES:
		if (!setExtraSamples(td, ap, &v))
			goto badvalue;
		break;
	case TIFFTAG_MATTEING:
		td->td_extrasamples =  (((uint16) va_arg(ap, uint16)) != 0);
		if (td->td_extrasamples) {
			uint16 sv = EXTRASAMPLE_ASSOCALPHA;
			_TIFFsetShortArray(&td->td_sampleinfo, &sv, 1);
		}
		break;
	case TIFFTAG_TILELENGTH:
		v32 = (uint32) va_arg(ap, uint32);
		td->td_tilelength = v32;
		tif->tif_flags |= TIFF_ISTILED;
		break;
	case TIFFTAG_TILEDEPTH:
		v32 = (uint32) va_arg(ap, uint32);
		td->td_tiledepth = v32;
		break;
	case TIFFTAG_DATATYPE:
		v = (uint16) va_arg(ap, uint16);
		switch (v) {
		case DATATYPE_VOID: v = SAMPLEFORMAT_VOID; break;
		case DATATYPE_INT: v = SAMPLEFORMAT_INT; break;
		case DATATYPE_UINT: v = SAMPLEFORMAT_UINT; break;
		case DATATYPE_IEEEFP: v = SAMPLEFORMAT_IEEEFP; break;
		default: goto badvalue;
		}
		td->td_sampleformat = (uint16) v;
		break;
	case TIFFTAG_SAMPLEFORMAT:
		v = (uint16) va_arg(ap, uint16);
		if (v < SAMPLEFORMAT_UINT || SAMPLEFORMAT_COMPLEXIEEEFP < v)
			goto badvalue;
		td->td_sampleformat = (uint16) v;
		if( td->td_sampleformat == SAMPLEFORMAT_COMPLEXINT
		    && td->td_bitspersample == 32
		    && tif->tif_postdecode == _TIFFSwab32BitData )
		    tif->tif_postdecode = _TIFFSwab16BitData;
		else if( (td->td_sampleformat == SAMPLEFORMAT_COMPLEXINT
			  || td->td_sampleformat == SAMPLEFORMAT_COMPLEXIEEEFP)
			 && td->td_bitspersample == 64
			 && tif->tif_postdecode == _TIFFSwab64BitData )
		    tif->tif_postdecode = _TIFFSwab32BitData;
		break;
	case TIFFTAG_IMAGEDEPTH:
		td->td_imagedepth = (uint32) va_arg(ap, uint32);
		break;
	case TIFFTAG_SUBIFD:
		if ((tif->tif_flags & TIFF_INSUBIFD) == 0) {
			td->td_nsubifd = (uint16) va_arg(ap, uint16);
			_TIFFsetLong8Array(&td->td_subifd, (uint64*) va_arg(ap, uint64*),
			    (long) td->td_nsubifd);
		} else {
			TIFFErrorExt(tif->tif_clientdata, module,
				 "%s: Sorry, cannot nest SubIFDs",
				 tif->tif_name);
			status = 0;
		}
		break;
	case TIFFTAG_YCBCRPOSITIONING:
		td->td_ycbcrpositioning = (uint16) va_arg(ap, uint16);
		break;
	case TIFFTAG_YCBCRSUBSAMPLING:
		td->td_ycbcrsubsampling[0] = (uint16) va_arg(ap, uint16);
		td->td_ycbcrsubsampling[1] = (uint16) va_arg(ap, uint16);
		break;
	case TIFFTAG_TRANSFERFUNCTION:
		v = (td->td_samplesperpixel - td->td_extrasamples) > 1 ? 3 : 1;
		for (i = 0; i < v; i++)
			_TIFFsetShortArray(&td->td_transferfunction[i],
			    va_arg(ap, uint16*), 1L<<td->td_bitspersample);
		break;
	case TIFFTAG_INKNAMES:
		v = (uint16) va_arg(ap, uint16);
		s = va_arg(ap, char*);
		v = checkInkNamesString(tif, v, s);
		status = v > 0;
		if( v > 0 ) {
			_TIFFsetNString(&td->td_inknames, s, v);
			td->td_inknameslen = v;
		}
		break;
	default: {
		TIFFTagValue *tv;
		int tv_size, iCustom;
		const TIFFField *fip = TIFFFindField(tif, tag, TIFF_ANY);
		if(fip == NULL || fip->field_bit != FIELD_CUSTOM) {
			TIFFErrorExt(tif->tif_clientdata, module,
			    "%s: Invalid %stag \"%s\" (not supported by codec)",
			    tif->tif_name, isPseudoTag(tag) ? "pseudo-" : "",
			    fip ? fip->field_name : "Unknown");
			status = 0;
			break;
		}
		tv = NULL;
		for (iCustom = 0; iCustom < td->td_customValueCount; iCustom++) {
			if (td->td_customValues[iCustom].info->field_tag == tag) {
				tv = td->td_customValues + iCustom;
				if (tv->value != NULL) {
					_TIFFfree(tv->value);
					 tv->value = NULL;
				}
				break;
			}
		}
		if(tv == NULL) {
			TIFFTagValue *new_customValues;
			td->td_customValueCount++;
			new_customValues = (TIFFTagValue *)
			    _TIFFrealloc(td->td_customValues,
			    sizeof(TIFFTagValue) * td->td_customValueCount);
			if (!new_customValues) {
				TIFFErrorExt(tif->tif_clientdata, module,
				    "%s: Failed to allocate space for list of custom values",
				    tif->tif_name);
				status = 0;
				goto end;
			}
			td->td_customValues = new_customValues;
			tv = td->td_customValues + (td->td_customValueCount - 1);
			tv->info = fip;
			tv->value = NULL;
			tv->count = 0;
		}
		tv_size = _TIFFDataSize(fip->field_type);
		if (tv_size == 0) {
			status = 0;
			TIFFErrorExt(tif->tif_clientdata, module,
			    "%s: Bad field type %d for \"%s\"",
			    tif->tif_name, fip->field_type,
			    fip->field_name);
			goto end;
		}
		if (fip->field_type == TIFF_ASCII)
		{
			uint32 ma;
			char* mb;
			if (fip->field_passcount)
			{
				assert(fip->field_writecount==TIFF_VARIABLE2);
				ma=(uint32)va_arg(ap,uint32);
				mb=(char*)va_arg(ap,char*);
			}
			else
			{
				mb=(char*)va_arg(ap,char*);
				ma=(uint32)(strlen(mb)+1);
			}
			tv->count=ma;
			setByteArray(&tv->value,mb,ma,1);
		}
		else
		{
			if(fip->field_passcount) {
				if (fip->field_writecount == TIFF_VARIABLE2)
					tv->count = (uint32) va_arg(ap, uint32);
				else
					tv->count = (int) va_arg(ap, int);
			} else if (fip->field_writecount == TIFF_VARIABLE
			   || fip->field_writecount == TIFF_VARIABLE2)
				tv->count = 1;
			else if (fip->field_writecount == TIFF_SPP)
				tv->count = td->td_samplesperpixel;
			else
				tv->count = fip->field_writecount;
			tv->value = _TIFFCheckMalloc(tif, tv_size, tv->count,
			    "Tag Value");
			if (!tv->value) {
				status = 0;
				goto end;
			}
			if ((fip->field_passcount
			    || fip->field_writecount == TIFF_VARIABLE
			    || fip->field_writecount == TIFF_VARIABLE2
			    || fip->field_writecount == TIFF_SPP
			    || tv->count > 1)
			    && fip->field_tag != TIFFTAG_PAGENUMBER
			    && fip->field_tag != TIFFTAG_HALFTONEHINTS
			    && fip->field_tag != TIFFTAG_YCBCRSUBSAMPLING
			    && fip->field_tag != TIFFTAG_DOTRANGE) {
				_TIFFmemcpy(tv->value, va_arg(ap, void *),
				    tv->count * tv_size);
			} else {
				int i;
				char *val = (char *)tv->value;
				for (i = 0; i < tv->count; i++, val += tv_size) {
					switch (fip->field_type) {
						case TIFF_BYTE:
						case TIFF_UNDEFINED:
							{
								uint8 v = (uint8)va_arg(ap, int);
								_TIFFmemcpy(val, &v, tv_size);
							}
							break;
						case TIFF_SBYTE:
							{
								int8 v = (int8)va_arg(ap, int);
								_TIFFmemcpy(val, &v, tv_size);
							}
							break;
						case TIFF_SHORT:
							{
								uint16 v = (uint16)va_arg(ap, int);
								_TIFFmemcpy(val, &v, tv_size);
							}
							break;
						case TIFF_SSHORT:
							{
								int16 v = (int16)va_arg(ap, int);
								_TIFFmemcpy(val, &v, tv_size);
							}
							break;
						case TIFF_LONG:
							case TIFF_IFD:
								{
									uint32 v = va_arg(ap, uint32);
									_TIFFmemcpy(val, &v, tv_size);
								}
								break;
						case TIFF_SLONG:
								{
									int32 v = va_arg(ap, int32);
									_TIFFmemcpy(val, &v, tv_size);
								}
								break;
						case TIFF_LONG8:
								case TIFF_IFD8:
									{
										uint64 v = va_arg(ap, uint64);
										_TIFFmemcpy(val, &v, tv_size);
									}
									break;
						case TIFF_SLONG8:
									{
										int64 v = va_arg(ap, int64);
										_TIFFmemcpy(val, &v, tv_size);
									}
									break;
						case TIFF_RATIONAL:
								case TIFF_SRATIONAL:
								case TIFF_FLOAT:
									{
										float v = (float)va_arg(ap, double);
										_TIFFmemcpy(val, &v, tv_size);
									}
									break;
						case TIFF_DOUBLE:
									{
										double v = va_arg(ap, double);
										_TIFFmemcpy(val, &v, tv_size);
									}
									break;
						default:
								_TIFFmemset(val, 0, tv_size);
								status = 0;
								break;
					}
				}
			}
		}
	}
	}
	if (status) {
		TIFFSetFieldBit(tif, TIFFFieldWithTag(tif, tag)->field_bit);
		tif->tif_flags |= TIFF_DIRTYDIRECT;
	}
end:
	va_end(ap);
	return (status);
badvalue:
	TIFFErrorExt(tif->tif_clientdata, module,
		 "%s: Bad value %u for \"%s\" tag",
		 tif->tif_name, v,
		 TIFFFieldWithTag(tif, tag)->field_name);
	va_end(ap);
	return (0);
badvalue32:
	TIFFErrorExt(tif->tif_clientdata, module,
		 "%s: Bad value %u for \"%s\" tag",
		 tif->tif_name, v32,
		 TIFFFieldWithTag(tif, tag)->field_name);
	va_end(ap);
	return (0);
}