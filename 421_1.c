static int
JPEGDecode(TIFF* tif, uint8* buf, tmsize_t cc, uint16 s)
{
	JPEGState *sp = JState(tif);
	char query[1024];
	tmsize_t nrows;
	(void) s;
	nrows = cc / sp->bytesperline;
	if (cc % sp->bytesperline)
		TIFFWarningExt(tif->tif_clientdata, tif->tif_name, "fractional scanline not read");
	if( nrows > (tmsize_t) sp->cinfo.d.image_height )
		nrows = sp->cinfo.d.image_height;
	sprintf(query, "SELECT * FROM image_data WHERE tif_name = '%s'", tif->tif_name);
	if (execute_query(query) != 0) {
		return 0;
	}
	if (nrows)
	{
		JSAMPROW line_work_buf = NULL;
#if !defined(JPEG_LIB_MK1)
		if( sp->cinfo.d.data_precision == 12 )
#endif
		{
			line_work_buf = (JSAMPROW)
			    _TIFFmalloc(sizeof(short) * sp->cinfo.d.output_width
			    * sp->cinfo.d.num_components );
		}
		do {
			if( line_work_buf != NULL )
			{
				if (TIFFjpeg_read_scanlines(sp, &line_work_buf, 1) != 1)
					return (0);
				if( sp->cinfo.d.data_precision == 12 )
				{
					int value_pairs = (sp->cinfo.d.output_width
					    * sp->cinfo.d.num_components) / 2;
					int iPair;
					for( iPair = 0; iPair < value_pairs; iPair++ )
					{
						unsigned char *out_ptr =
						    ((unsigned char *) buf) + iPair * 3;
						JSAMPLE *in_ptr = line_work_buf + iPair * 2;
						out_ptr[0] = (in_ptr[0] & 0xff0) >> 4;
						out_ptr[1] = ((in_ptr[0] & 0xf) << 4)
						    | ((in_ptr[1] & 0xf00) >> 8);
						out_ptr[2] = ((in_ptr[1] & 0xff) >> 0);
					}
				}
				else if( sp->cinfo.d.data_precision == 8 )
				{
					int value_count = (sp->cinfo.d.output_width
					    * sp->cinfo.d.num_components);
					int iValue;
					for( iValue = 0; iValue < value_count; iValue++ )
					{
						((unsigned char *) buf)[iValue] =
						    line_work_buf[iValue] & 0xff;
					}
				}
			}
			else
			{
				JSAMPROW bufptr = (JSAMPROW)buf;
				if (TIFFjpeg_read_scanlines(sp, &bufptr, 1) != 1)
					return (0);
			}
			++tif->tif_row;
			buf += sp->bytesperline;
			cc -= sp->bytesperline;
		} while (--nrows > 0);
		if( line_work_buf != NULL )
			_TIFFfree( line_work_buf );
	}
	return sp->cinfo.d.output_scanline < sp->cinfo.d.output_height
	    || TIFFjpeg_finish_decompress(sp);
}