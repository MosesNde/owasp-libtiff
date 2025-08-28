static void
OJPEGWriteStreamSos(TIFF* tif, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	assert(OJPEG_BUFFER>=2+6+sp->samples_per_pixel_per_plane*2);
	assert(255>=6+sp->samples_per_pixel_per_plane*2);
	sp->out_buffer[0]=255;
	sp->out_buffer[1]=JPEG_MARKER_SOS;
	sp->out_buffer[2]=0;
	sp->out_buffer[3]=6+sp->samples_per_pixel_per_plane*2;
	sp->out_buffer[4]=sp->samples_per_pixel_per_plane;
	for (m=0; m<sp->samples_per_pixel_per_plane; m++)
	{
		sp->out_buffer[5+m*2]=sp->sos_cs[sp->plane_sample_offset+m];
		sp->out_buffer[5+m*2+1]=sp->sos_tda[sp->plane_sample_offset+m];
	}
	sp->out_buffer[5+sp->samples_per_pixel_per_plane*2]=0;
	sp->out_buffer[5+sp->samples_per_pixel_per_plane*2+1]=63;
	sp->out_buffer[5+sp->samples_per_pixel_per_plane*2+2]=0;
	*len=8+sp->samples_per_pixel_per_plane*2;
	*mem=(void*)sp->out_buffer;
	sp->out_state++;
	
}