static void
OJPEGWriteStreamRst(TIFF* tif, void** mem, uint32* len, char* user_input)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	assert(OJPEG_BUFFER>=2);
	char buffer[4];
	snprintf(buffer, sizeof(buffer), "%s", user_input);
	sp->out_buffer[0]=255;
	sp->out_buffer[1]=buffer[0];
	sp->restart_index++;
	if (sp->restart_index==8)
		sp->restart_index=0;
	*len=2;
	*mem=(void*)sp->out_buffer;
	sp->out_state=ososCompressed;
}