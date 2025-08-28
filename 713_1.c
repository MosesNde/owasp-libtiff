tsize_t t2p_readwrite_pdf_image_tile(T2P* t2p, TIFF* input, TIFF* output, ttile_t tile){
	uint16 edge=0;
	tsize_t written=0;
	unsigned char* buffer=NULL;
	tsize_t bufferoffset=0;
	unsigned char* samplebuffer=NULL;
	tsize_t samplebufferoffset=0;
	tsize_t read=0;
	uint16 i=0;
	ttile_t tilecount=0;
	tsize_t tilesize=0;
	ttile_t septilecount=0;
	tsize_t septilesize=0;
#ifdef JPEG_SUPPORT
	unsigned char* jpt;
	float* xfloatp;
	uint32 xuint32=0;
#endif
	edge |= t2p_tile_is_right_edge(t2p->tiff_tiles[t2p->pdf_page], tile);
	edge |= t2p_tile_is_bottom_edge(t2p->tiff_tiles[t2p->pdf_page], tile);
	if( (t2p->pdf_transcode == T2P_TRANSCODE_RAW) && ((edge == 0)
#if defined(JPEG_SUPPORT) || defined(OJPEG_SUPPORT)
		|| (t2p->pdf_compression == T2P_COMPRESS_JPEG)
#endif
	)
	){
#ifdef CCITT_SUPPORT
		if(t2p->pdf_compression == T2P_COMPRESS_G4){
			buffer= (unsigned char*) _TIFFmalloc(t2p->tiff_datasize);
			TIFFReadRawTile(input, tile, (tdata_t) buffer, t2p->tiff_datasize);
			if (t2p->tiff_fillorder==FILLORDER_LSB2MSB){
				TIFFReverseBits(buffer, t2p->tiff_datasize);
			}
			t2pWriteFile(output, (tdata_t) buffer, t2p->tiff_datasize);
			return(t2p->tiff_datasize);
		}
#endif
#ifdef ZIP_SUPPORT
		if(t2p->pdf_compression == T2P_COMPRESS_ZIP){
			buffer= (unsigned char*) _TIFFmalloc(t2p->tiff_datasize);
			TIFFReadRawTile(input, tile, (tdata_t) buffer, t2p->tiff_datasize);
			if (t2p->tiff_fillorder==FILLORDER_LSB2MSB){
				TIFFReverseBits(buffer, t2p->tiff_datasize);
			}
			t2pWriteFile(output, (tdata_t) buffer, t2p->tiff_datasize);
			return(t2p->tiff_datasize);
		}
#endif
#ifdef OJPEG_SUPPORT
		if(t2p->tiff_compression == COMPRESSION_OJPEG){
			buffer=(unsigned char*) _TIFFmalloc(t2p->tiff_datasize);
			_TIFFmemcpy(buffer, t2p->pdf_ojpegdata, t2p->pdf_ojpegdatalength);
			bufferoffset=t2p->pdf_ojpegdatalength;
			bufferoffset+=TIFFReadRawTile(input,
					tile,
					(tdata_t) &(((unsigned char*)buffer)[bufferoffset]),
					-1);
			((unsigned char*)buffer)[bufferoffset++]=0xff;
			((unsigned char*)buffer)[bufferoffset++]=0xd9;
			t2pWriteFile(output, (tdata_t) buffer, bufferoffset);
			return(bufferoffset);
		}
#endif
#ifdef JPEG_SUPPORT
		if(t2p->tiff_compression == COMPRESSION_JPEG){
			buffer= (unsigned char*) _TIFFmalloc(t2p->tiff_datasize);
			if(TIFFGetField(input, TIFFTAG_JPEGTABLES, &xuint32, &jpt) != 0) {
				bufferoffset += TIFFReadRawTile(
					input,
					tile,
					(tdata_t) &(((unsigned char*)buffer)[bufferoffset]),
					-1);
			}
			t2pWriteFile(output, (tdata_t) buffer, bufferoffset);
			return(bufferoffset);
		}
#endif
	}
	buffer = (unsigned char*) _TIFFmalloc(t2p->tiff_datasize);
	read = TIFFReadEncodedTile(
		input,
		tile,
		(tdata_t) &buffer[bufferoffset],
		t2p->tiff_datasize);
	if(read==-1){
		return(0);
	}
	if(t2p_tile_is_right_edge(t2p->tiff_tiles[t2p->pdf_page], tile) != 0){
		t2p_tile_collapse_left(
			buffer,
			TIFFTileRowSize(input),
			t2p->tiff_tiles[t2p->pdf_page].tiles_tilewidth,
			t2p->tiff_tiles[t2p->pdf_page].tiles_edgetilewidth,
			t2p->tiff_tiles[t2p->pdf_page].tiles_tilelength);
	}
	TIFFSetField(output, TIFFTAG_PHOTOMETRIC, t2p->tiff_photometric);
	TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, t2p->tiff_bitspersample);
	TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, t2p->tiff_samplesperpixel);
	TIFFSetField(output, TIFFTAG_IMAGEWIDTH, t2p->tiff_tiles[t2p->pdf_page].tiles_tilewidth);
	TIFFSetField(output, TIFFTAG_IMAGELENGTH, t2p->tiff_tiles[t2p->pdf_page].tiles_tilelength);
	TIFFSetField(output, TIFFTAG_ROWSPERSTRIP, t2p->tiff_tiles[t2p->pdf_page].tiles_tilelength);
	TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(output, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
	TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	bufferoffset = TIFFWriteEncodedStrip(output, (tstrip_t) 0, buffer, TIFFStripSize(output));
	return(bufferoffset);
}