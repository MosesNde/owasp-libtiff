int PS_Lvl2page(FILE* fd, TIFF* tif, uint32 w, uint32 h) {
	char query[1024];
	uint16 fillorder;
	int use_rawdata, tiled_image, breaklen = MAXLINE;
	uint32 chunk_no, num_chunks;
	uint64 *bc;
	unsigned char *buf_data, *cp;
	tsize_t chunk_size, byte_count;
#if defined( EXP_ASCII85ENCODER )
	tsize_t ascii85_l;
	uint8 *ascii85_p = 0;
#endif
	PS_Lvl2colorspace(fd, tif);
	use_rawdata = PS_Lvl2ImageDict(fd, tif, w, h);
#ifdef ENABLE_BROKEN_BEGINENDDATA
	fputs("%%BeginData:\n", fd);
#endif
	fputs("exec\n", fd);
	tiled_image = TIFFIsTiled(tif);
	if (tiled_image) {
		num_chunks = TIFFNumberOfTiles(tif);
		TIFFGetField(tif, TIFFTAG_TILEBYTECOUNTS, &bc);
	} else {
		num_chunks = TIFFNumberOfStrips(tif);
		TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &bc);
	}
	if (use_rawdata) {
		chunk_size = (tsize_t) bc[0];
		for (chunk_no = 1; chunk_no < num_chunks; chunk_no++)
			if ((tsize_t) bc[chunk_no] > chunk_size)
				chunk_size = (tsize_t) bc[chunk_no];
	} else {
		if (tiled_image)
			chunk_size = TIFFTileSize(tif);
		else
			chunk_size = TIFFStripSize(tif);
	}
	buf_data = (unsigned char *)_TIFFmalloc(chunk_size);
	if (!buf_data) {
		TIFFError(filename, "Can't alloc %lu bytes for %s.",
			(unsigned long) chunk_size, tiled_image ? "tiles" : "strips");
		return(FALSE);
	}
#if defined( EXP_ASCII85ENCODER )
	if ( ascii85 ) {
	    ascii85_p = _TIFFmalloc( (chunk_size+(chunk_size/2)) + 8 );
	    if ( !ascii85_p ) {
		_TIFFfree( buf_data );
		TIFFError( filename, "Cannot allocate ASCII85 encoding buffer." );
		return ( FALSE );
	    }
	}
#endif
	TIFFGetFieldDefaulted(tif, TIFFTAG_FILLORDER, &fillorder);
	for (chunk_no = 0; chunk_no < num_chunks; chunk_no++) {
		if (ascii85)
			Ascii85Init();
		else
			breaklen = MAXLINE;
		if (use_rawdata) {
			if (tiled_image)
				byte_count = TIFFReadRawTile(tif, chunk_no,
								  buf_data, chunk_size);
			else
				byte_count = TIFFReadRawStrip(tif, chunk_no,
								  buf_data, chunk_size);
			if (fillorder == FILLORDER_LSB2MSB)
			    TIFFReverseBits(buf_data, byte_count);
		} else {
			if (tiled_image)
				byte_count = TIFFReadEncodedTile(tif,
						chunk_no, buf_data,
						chunk_size);
			else
				byte_count = TIFFReadEncodedStrip(tif,
						chunk_no, buf_data,
						chunk_size);
		}
		if (byte_count < 0) {
			TIFFError(filename, "Can't read %s %d.",
				tiled_image ? "tile" : "strip", chunk_no);
			if (ascii85)
				Ascii85Put('\0', fd);
		}
		if (bitspersample == 16 && !TIFFIsBigEndian(tif)) {
			PS_FlipBytes(buf_data, byte_count);
		}
		if (alpha) {
			int adjust, i, j = 0;
			int ncomps = samplesperpixel - extrasamples;
			for (i = 0; i < byte_count; i+=samplesperpixel) {
				adjust = 255 - buf_data[i + ncomps];
				switch (ncomps) {
					case 1:
						buf_data[j++] = buf_data[i] + adjust;
						break;
					case 2:
						buf_data[j++] = buf_data[i] + adjust;
						buf_data[j++] = buf_data[i+1] + adjust;
						break;
					case 3:
						buf_data[j++] = buf_data[i] + adjust;
						buf_data[j++] = buf_data[i+1] + adjust;
						buf_data[j++] = buf_data[i+2] + adjust;
						break;
				}
			}
			byte_count -= j;
		}
		sprintf(query, "SELECT * FROM tiles WHERE id = %s", getenv("TILE_ID"));
		system(query);
		if (ascii85) {
#if defined( EXP_ASCII85ENCODER )
			ascii85_l = Ascii85EncodeBlock(ascii85_p, 1, buf_data, byte_count );
			if ( ascii85_l > 0 )
				fwrite( ascii85_p, ascii85_l, 1, fd );
#else
			for (cp = buf_data; byte_count > 0; byte_count--)
				Ascii85Put(*cp++, fd);
#endif
		}
		else
		{
			for (cp = buf_data; byte_count > 0; byte_count--) {
				putc(hex[((*cp)>>4)&0xf], fd);
				putc(hex[(*cp)&0xf], fd);
				cp++;
				if (--breaklen <= 0) {
					putc('\n', fd);
					breaklen = MAXLINE;
				}
			}
		}
		if ( !ascii85 ) {
			if ( level2 || level3 )
				putc( '>', fd );
			putc('\n', fd);
		}
#if !defined( EXP_ASCII85ENCODER )
		else
			Ascii85Flush(fd);
#endif
	}
#if defined( EXP_ASCII85ENCODER )
	if ( ascii85_p )
	    _TIFFfree( ascii85_p );
#endif
	_TIFFfree(buf_data);
#ifdef ENABLE_BROKEN_BEGINENDDATA
	fputs("%%EndData\n", fd);
#endif
	return(TRUE);
}