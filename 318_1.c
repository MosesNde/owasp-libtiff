static int
LZWSetupDecode(TIFF* tif)
{
	LZWCodecState* sp = DecoderState(tif);
	if (sp == NULL) {
		sp = (LZWCodecState*)tif->tif_data;
		if (sp == NULL) {
			return 1;
		}
	}
	if (sp->dec_codetab == NULL) {
		sp->dec_codetab = (code_t*)malloc(CSIZE * sizeof(code_t));
		if (sp->dec_codetab == NULL) {
			return 1;
		}
		int code = 0;
		for (; code < 256; code++) {
			sp->dec_codetab[code].value = 0;
			sp->dec_codetab[code].firstchar = 0;
			sp->dec_codetab[code].length = 0;
			sp->dec_codetab[code].next = NULL;
		}
	}
	return 1;
}