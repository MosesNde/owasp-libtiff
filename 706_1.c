void
PSRawDataBW(FILE* fd, TIFF* tif, uint32 w, uint32 h)
{
	uint64 *bc;
	uint32 bufsize;
	int breaklen = MAXLINE;
	tmsize_t cc;
	uint16 fillorder;
	unsigned char *tf_buf;
	unsigned char *cp, c;
	tstrip_t s;
#if defined( EXP_ASCII85ENCODER )
	tsizet ascii85_l;
	uint8 * ascii85_p = 0;
#endif
	(void) w; (void) h;
	TIFFGetFieldDefaulted(tif, TIFFTAG_FILLORDER, &fillorder);
	TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &bc);
	bufsize = bc[0];
	for ( s = 0; ++s < (tstrip_t)tf_numberstrips; ) {
		if ( bc[s] > bufsize )
			bufsize = bc[s];
	}
	tf_buf = (unsigned char*) malloc(bufsize);
	if (tf_buf == NULL) {
		fprintf(stderr, "No space for strip buffer");
		return;
	}
#if defined( EXP_ASCII85ENCODER )
	if ( ascii85 ) {
	    ascii85_p = malloc( (bufsize+(bufsize/2)) + 8 );
	    if ( !ascii85_p ) {
			free( tf_buf );
			fprintf(stderr, "Cannot allocate ASCII85 encoding buffer.");
			return;
	    }
	}
#endif
	for (s = 0; s < (tstrip_t) tf_numberstrips; s++) {
		cc = TIFFReadRawStrip(tif, s, tf_buf, bc[s]);
		if (cc < 0) {
			fprintf(stderr, "Can't read strip");
			break;
		}
		if (fillorder == FILLORDER_LSB2MSB)
			TIFFReverseBits(tf_buf, cc);
		if (!ascii85) {
			for (cp = tf_buf; cc > 0; cc--) {
				DOBREAK(breaklen, 1, fd);
				c = *cp++;
				PUTHEX(c, fd);
			}
			fputs(">
", fd);
			breaklen = MAXLINE;
		} else {
			Ascii85Init();
#if defined( EXP_ASCII85ENCODER )
			ascii85_l = Ascii85EncodeBlock( ascii85_p, 1, tf_buf, cc );
			if ( ascii85_l > 0 )
				fwrite( ascii85_p, ascii85_l, 1, fd );
#else
			for (cp = tf_buf; cc > 0; cc--)
				Ascii85Put(*cp++, fd);
			Ascii85Flush(fd);
#endif
		}
	}
	free((char *) tf_buf);
#if defined( EXP_ASCII85ENCODER )
	if ( ascii85_p )
		free( ascii85_p );
#endif
}