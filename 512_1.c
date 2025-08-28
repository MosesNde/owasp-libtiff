static int
LogLuvInitState(TIFF* tif)
{
	static const char module[] = "LogLuvInitState";
	TIFFDirectory* td = &tif->tif_dir;
	LogLuvState* sp = DecoderState(tif);
	assert(sp != NULL);
	assert(td->td_photometric == PHOTOMETRIC_LOGLUV);
	if (td->td_planarconfig != PLANARCONFIG_CONTIG) {
		TIFFErrorExt(tif->tif_clientdata, module,
			"SGILog compression cannot handle non-contiguous data");
		return (0);
	}
	if (sp->user_datafmt == SGILOGDATAFMT_UNKNOWN)
		sp->user_datafmt = LogLuvGuessDataFmt(td);
	switch (sp->user_datafmt) {
	case SGILOGDATAFMT_FLOAT:
		sp->pixel_size = 3*sizeof (float);
		break;
	case SGILOGDATAFMT_16BIT:
		sp->pixel_size = 3*sizeof (int16);
		break;
	case SGILOGDATAFMT_RAW:
		sp->pixel_size = sizeof (uint32);
		break;
	case SGILOGDATAFMT_8BIT:
		sp->pixel_size = 3*sizeof (uint8);
		break;
	default:
		TIFFErrorExt(tif->tif_clientdata, module,
			"No support for converting user data format to LogLuv");
		return (0);
	}
	sp->tbuflen = multiply_ms(td->td_imagewidth, td->td_rowsperstrip);
	if (multiply_ms(sp->tbuflen, sizeof (uint32)) == 0 ||
		(sp->tbuf = (uint8*) _TIFFmalloc(sp->tbuflen * sizeof (uint32))) == NULL) {
		TIFFErrorExt(tif->tif_clientdata, module, "No space for SGILog translation buffer");
		return (0);
	}
	return (1);
}