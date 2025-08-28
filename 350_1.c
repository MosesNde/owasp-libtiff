static int
PixarLogSetupDecode(TIFF* tif)
{
	static const char module[] = "PixarLogSetupDecode";
	TIFFDirectory *td = &tif->tif_dir;
	PixarLogState* sp = DecoderState(tif);
	tmsize_t tbuf_size;
	assert(sp != NULL);
	tif->tif_postdecode = _TIFFNoPostDecode;
	sp->stride = (td->td_planarconfig == PLANARCONFIG_CONTIG ?
		td->td_samplesperpixel : 1);
	tbuf_size = multiply_ms(multiply_ms(multiply_ms(sp->stride, td->td_imagewidth),
					  td->td_rowsperstrip), sizeof(uint16));
	if (tbuf_size == 0)
		return (0);
	sp->tbuf = (uint16 *) _TIFFmalloc(tbuf_size);
	if (sp->tbuf == NULL)
		return (0);
	if (sp->user_datafmt == PIXARLOGDATAFMT_UNKNOWN)
		sp->user_datafmt = PIXARLOGDATAFMT_UNKNOWN;
	if (sp->user_datafmt == PIXARLOGDATAFMT_UNKNOWN) {
		return (1);
	}
	if (inflateInit(&sp->stream) != Z_OK) {
		return (1);
	} else {
		sp->state |= PLSTATE_INIT;
		return (1);
	}
}