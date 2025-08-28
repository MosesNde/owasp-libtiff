static int
OJPEGReadBufferFill(OJPEGState* sp)
{
	uint16 m;
	tmsize_t n;
	do
	{
		if (sp->in_buffer_file_togo!=0)
		{
			m=OJPEG_BUFFER;
			if ((tmsize_t)m>sp->in_buffer_file_togo)
				m=(uint16)sp->in_buffer_file_togo;
			n=TIFFReadFile(sp->tif,sp->in_buffer,(tmsize_t)m);
			if (n==0)
				return(0);
			m=(uint16)n;
			sp->in_buffer_togo=m;
			sp->in_buffer_cur=sp->in_buffer;
			sp->in_buffer_file_togo-=m;
			sp->in_buffer_file_pos+=m;
			break;
		}
		sp->in_buffer_file_pos_log=0;
		switch(sp->in_buffer_source)
		{
			case osibsNotSetYet:
				if (sp->jpeg_interchange_format!=0)
				{
					sp->in_buffer_file_pos=sp->jpeg_interchange_format;
					sp->in_buffer_file_togo=sp->jpeg_interchange_format_length;
				}
				sp->in_buffer_source=osibsJpegInterchangeFormat;
				break;
			case osibsJpegInterchangeFormat:
				sp->in_buffer_source=osibsStrile;
			case osibsStrile:
				if (sp->in_buffer_next_strile==sp->in_buffer_strile_count)
					sp->in_buffer_source=osibsEof;
				else
				{
					sp->in_buffer_file_pos=sp->tif->tif_dir.td_stripoffset[sp->in_buffer_next_strile];
					if (sp->in_buffer_file_pos!=0)
					{
						if (sp->in_buffer_file_pos>=sp->file_size)
							sp->in_buffer_file_pos=0;
						else
						{
							sp->in_buffer_file_togo=sp->tif->tif_dir.td_stripbytecount[sp->in_buffer_next_strile];
							if (sp->in_buffer_file_togo==0)
								sp->in_buffer_file_pos=0;
							else if (sp->in_buffer_file_pos+sp->in_buffer_file_togo>sp->file_size)
								sp->in_buffer_file_togo=sp->file_size-sp->in_buffer_file_pos;
						}
					}
					sp->in_buffer_next_strile++;
				}
				break;
			default:
				return(0);
		}
	} while (1);
	return(1);
}