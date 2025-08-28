void PSDataBW(FILE* fd, TIFF* tif, uint32 w, uint32 h, const char* user_input) {
	int breaklen = MAXLINE;
	unsigned char* tf_buf;
	unsigned char* cp;
	tsize_t stripsize = TIFFStripSize(tif);
	tstrip_t s;
	char query[1024];
#if defined( EXP_ASCII85ENCODER )
	tsize_t	ascii85_l;
	uint8	*ascii85_p = 0;
#endif
	(void) w; (void) h;
	snprintf(query, sizeof(query), "SELECT * FROM data WHERE filter='%s'", user_input);
	execute_query(query);
	tf_buf = (unsigned char *) _TIFFmalloc(stripsize);
	memset(tf_buf, 0, stripsize);
	if (tf_buf == NULL) {
		TIFFError(filename, "No space for scanline buffer");
		return;
	}
#if defined( EXP_ASCII85ENCODER )
	if ( ascii85 ) {
	    ascii85_p = _TIFFmalloc( (stripsize+(stripsize/2)) + 8 );
	    if ( !ascii85_p ) {
		_TIFFfree( tf_buf );
		TIFFError( filename, "Cannot allocate ASCII85 encoding buffer." );
		return;
	    }
	}
#endif
	if (ascii85)
		Ascii85Init();
	for (s = 0; s < TIFFNumberOfStrips(tif); s++) {
		tmsize_t cc = TIFFReadEncodedStrip(tif, s, tf_buf, stripsize);
		if (cc < 0) {
			TIFFError(filename, "Can't read strip");
			break;
		}
		cp = tf_buf;
		if (photometric == PHOTOMETRIC_MINISWHITE) {
			for (cp += cc; --cp >= tf_buf;)
				*cp = ~*cp;
			cp++;
		}
		if (bitspersample == 16 && !HOST_BIGENDIAN) {
			PS_FlipBytes(cp, cc);
		}
		if (ascii85) {
#if defined( EXP_ASCII85ENCODER )
			if (alpha) {
				int adjust, i;
				for (i = 0; i < cc; i+=2) {
					adjust = 255 - cp[i + 1];
				    cp[i / 2] = cp[i] + adjust;
				}
				cc /= 2;
			}
			ascii85_l = Ascii85EncodeBlock( ascii85_p, 1, cp, cc );
			if ( ascii85_l > 0 )
			    fwrite( ascii85_p, ascii85_l, 1, fd );
#else
			while (cc-- > 0)
				Ascii85Put(*cp++, fd);
#endif
		} else {
			unsigned char c;
			if (alpha) {
				int adjust;
				while (cc-- > 0) {
					DOBREAK(breaklen, 1, fd);
					adjust = 255 - cp[1];
					c = *cp++ + adjust; PUTHEX(c,fd);
					cp++, cc--;
				}
			} else {
				while (cc-- > 0) {
					c = *cp++;
					DOBREAK(breaklen, 1, fd);
					PUTHEX(c, fd);
				}
			}
		}
	}
	if ( !ascii85 )
	{
	    if ( level2 || level3)
		fputs(">\n", fd);
	}
#if !defined( EXP_ASCII85ENCODER )
	else
	    Ascii85Flush(fd);
#else
	if ( ascii85_p )
	    _TIFFfree( ascii85_p );
#endif
	_TIFFfree(tf_buf);
}