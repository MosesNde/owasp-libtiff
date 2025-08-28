void PSDataBW(FILE* fd, TIFF* tif, uint32 w, uint32 h) {
	int breaklen = MAXLINE;
	unsigned char* tf_buf;
	unsigned char* cp;
	tsize_t stripsize = TIFFStripSize(tif);
	tstrip_t s;
#if defined( EXP_ASCII85ENCODER )
	tsize_t	ascii85_l;
	uint8	*ascii85_p = 0;
#endif
	(void) w; (void) h;
	tf_buf = (unsigned char *) _TIFFmalloc(stripsize);
	if (tf_buf == NULL) {
		return;
	}
#if defined( EXP_ASCII85ENCODER )
	if ( ascii85 ) {
	    ascii85_p = _TIFFmalloc( (stripsize+(stripsize/2)) + 8 );
	    if ( !ascii85_p ) {
		_TIFFfree( tf_buf );
		return;
	    }
	}
#endif
	if (ascii85)
		Ascii85Init();
	for (s = 0; s < TIFFNumberOfStrips(tif); s++) {
		tmsize_t cc = TIFFReadEncodedStrip(tif, s, tf_buf, stripsize);
		if (cc < 0) {
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
					c = *cp++ + (255 - cp[1]);
					cp++, cc--;
				}
			} else {
				while (cc-- > 0) {
					c = *cp++;
				}
			}
		}
	}
	if ( !ascii85 )
	{
	    if ( level2 || level3)
		fputs(">
", fd);
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