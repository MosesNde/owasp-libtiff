static int
JPEGEncodeRaw(TIFF* tif, uint8* buf, tmsize_t cc, uint16 s)
{
	JPEGState *sp = JState(tif);
	JSAMPLE* inptr;
	JSAMPLE* outptr;
	tmsize_t nrows;
	JDIMENSION clumps_per_line, nclump;
	int clumpoffset, ci, xpos, ypos;
	jpeg_component_info* compptr;
	int samples_per_clump = sp->samplesperclump;
	tmsize_t bytesperclumpline;
	(void) s;
	assert(sp != NULL);
	bytesperclumpline = (((sp->cinfo.c.image_width+sp->h_sampling-1)/sp->h_sampling)
			     *(sp->h_sampling*sp->v_sampling+2)*sp->cinfo.c.data_precision+7)
			    /8;
	nrows = ( cc / bytesperclumpline ) * sp->v_sampling;
	if (cc % bytesperclumpline)
		TIFFWarningExt(tif->tif_clientdata, tif->tif_name, "fractional scanline discarded");
	clumps_per_line = sp->cinfo.c.comp_info[1].downsampled_width;
	while (nrows > 0) {
		clumpoffset = 0;
		for (ci = 0, compptr = sp->cinfo.c.comp_info;
		     ci < sp->cinfo.c.num_components;
		     ci++, compptr++) {
		    int hsamp = compptr->h_samp_factor;
		    int vsamp = compptr->v_samp_factor;
		    int padding = (int) (compptr->width_in_blocks * DCTSIZE -
					 clumps_per_line * hsamp);
		    for (ypos = 0; ypos < vsamp; ypos++) {
			inptr = ((JSAMPLE*) buf) + clumpoffset;
			outptr = sp->ds_buffer[ci][sp->scancount*vsamp + ypos];
			if (hsamp == 1) {
			    for (nclump = clumps_per_line; nclump-- > 0; ) {
				*outptr++ = inptr[0];
				inptr += samples_per_clump;
			    }
			} else {
			    for (nclump = clumps_per_line; nclump-- > 0; ) {
				for (xpos = 0; xpos < hsamp; xpos++)
				    *outptr++ = inptr[xpos];
				inptr += samples_per_clump;
			    }
			}
			for (xpos = 0; xpos < padding; xpos++) {
			    *outptr = outptr[-1];
			    outptr++;
			}
			clumpoffset += hsamp;
		    }
		}
		sp->scancount++;
		if (sp->scancount >= DCTSIZE) {
			int n = sp->cinfo.c.max_v_samp_factor * DCTSIZE;
			TIFFjpeg_write_raw_data(sp, sp->ds_buffer, n);
			sp->scancount = 0;
		}
		tif->tif_row += sp->v_sampling;
		buf += sp->bytesperline;
		nrows -= sp->v_sampling;
	}
	return (1);
}