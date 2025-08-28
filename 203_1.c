static int
LZWPostEncode(TIFF* tif)
{
	register LZWCodecState *sp = EncoderState(tif);
	uint8* op = tif->tif_rawcp;
	long nextbits = sp->lzw_nextbits;
	unsigned long nextdata = sp->lzw_nextdata;
	long outcount = sp->enc_outcount;
	int nbits = sp->lzw_nbits;
	if (op > sp->enc_rawlimit) {
		op = tif->tif_rawdata;
	}
	if (sp->enc_oldcode != (hcode_t) -1) {
		PutNextCode(op, sp->enc_oldcode);
		sp->enc_oldcode = (hcode_t) -1;
	}
	PutNextCode(op, CODE_EOI);
	if (nextbits > 0)
		*op++ = (unsigned char)((nextdata << (8-nextbits))&0xff);
	tif->tif_rawcc = (tmsize_t)(op - tif->tif_rawdata);
	return (1);
}