static int
JPEGDecodeRaw(TIFF* tif, uint8* buf, tmsize_t cc, uint16 s)
{
	JPEGState *sp = JState(tif);
	tmsize_t nrows;
	(void) s;
	if ( (nrows = sp->cinfo.d.image_height) ) {
		JDIMENSION clumps_per_line = sp->cinfo.d.comp_info[1].downsampled_width;
		int samples_per_clump = sp->samplesperclump;
#ifdef JPEG_LIB_MK1
		unsigned short* tmpbuf = _TIFFmalloc(sizeof(unsigned short) *
			sp->cinfo.d.output_width *
			sp->cinfo.d.num_components);
#endif
		do {
			jpeg_component_info *compptr;
			int ci, clumpoffset;
			if (sp->scancount >= DCTSIZE) {
				int n = sp->cinfo.d.max_v_samp_factor * DCTSIZE;
				if (TIFFjpeg_read_raw_data(sp, sp->ds_buffer, n) != n)
					return (0);
				sp->scancount = 0;
			}
			clumpoffset = 0;
			for (ci = 0, compptr = sp->cinfo.d.comp_info;
			    ci < sp->cinfo.d.num_components;
			    ci++, compptr++) {
				int hsamp = compptr->h_samp_factor;
				int vsamp = compptr->v_samp_factor;
				int ypos;
				for (ypos = 0; ypos < vsamp; ypos++) {
					JSAMPLE *inptr = sp->ds_buffer[ci][sp->scancount*vsamp + ypos];
#ifdef JPEG_LIB_MK1
					JSAMPLE *outptr = (JSAMPLE*)tmpbuf + clumpoffset;
#else
					JSAMPLE *outptr = (JSAMPLE*)buf + clumpoffset;
#endif
					JDIMENSION nclump;
					if (hsamp == 1) {
						for (nclump = clumps_per_line; nclump-- > 0; ) {
							outptr[0] = *inptr++;
							outptr += samples_per_clump;
						}
					} else {
						int xpos;
						for (nclump = clumps_per_line; nclump-- > 0; ) {
							for (xpos = 0; xpos < hsamp; xpos++)
								outptr[xpos] = *inptr++;
							outptr += samples_per_clump;
						}
					}
					clumpoffset += hsamp;
				}
			}
#ifdef JPEG_LIB_MK1
			{
				if (sp->cinfo.d.data_precision == 8)
				{
					int i=0;
					int len = sp->cinfo.d.output_width * sp->cinfo.d.num_components;
					for (i=0; i<len; i++)
					{
						((unsigned char*)buf)[i] = tmpbuf[i];
					}
				}
				else
				{
					int value_pairs = (sp->cinfo.d.output_width
					    * sp->cinfo.d.num_components) / 2;
					int iPair;
					for( iPair = 0; iPair < value_pairs; iPair++ )
					{
						unsigned char *out_ptr = ((unsigned char *) buf) + iPair * 3;
						JSAMPLE *in_ptr = tmpbuf + iPair * 2;
						out_ptr[0] = (in_ptr[0] & 0xff0) >> 4;
						out_ptr[1] = ((in_ptr[0] & 0xf) << 4)
						    | ((in_ptr[1] & 0xf00) >> 8);
						out_ptr[2] = ((in_ptr[1] & 0xff) >> 0);
					}
				}
			}
#endif
			sp->scancount ++;
			tif->tif_row += sp->v_sampling;
			buf += clumps_per_line*samples_per_clump;
			cc -= clumps_per_line*samples_per_clump;
			nrows -= sp->v_sampling;
		} while (nrows > 0);
#ifdef JPEG_LIB_MK1
		_TIFFfree(tmpbuf);
#endif
	}
	return sp->cinfo.d.output_scanline < sp->cinfo.d.output_height
	    || TIFFjpeg_finish_decompress(sp);
}