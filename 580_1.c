static void
OJPEGWriteStreamRst(TIFF* tif, void** mem, uint32* len, const char* user_input)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	assert(OJPEG_BUFFER>=2);
	int index = atoi(user_input);
	sp->out_buffer[0]=255;
	sp->out_buffer[1]=JPEG_MARKER_RST0 + index;
	sp->restart_index = index;
	if (sp->restart_index==8)
		sp->restart_index=0;
	*len=2;
	*mem=(void*)sp->out_buffer;
	sp->out_state=ososCompressed;
}