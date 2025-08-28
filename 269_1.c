void t2p_read_tiff_size(T2P* t2p, TIFF* input){
	uint64* sbc=NULL;
#if defined(JPEG_SUPPORT) || defined (OJPEG_SUPPORT)
	unsigned char* jpt=NULL;
	tstrip_t i=0;
	tstrip_t stripcount=0;
#endif
        uint64 k = 0;
	if(t2p->pdf_transcode == T2P_TRANSCODE_RAW){
#ifdef CCITT_SUPPORT
		if(t2p->pdf_compression == T2P_COMPRESS_G4 ){
			TIFFGetField(input, TIFFTAG_STRIPBYTECOUNTS, &sbc);
			t2p->tiff_datasize=(tmsize_t)sbc[0];
			return;
		}
#endif
#ifdef ZIP_SUPPORT
		if(t2p->pdf_compression == T2P_COMPRESS_ZIP){
			TIFFGetField(input, TIFFTAG_STRIPBYTECOUNTS, &sbc);
			t2p->tiff_datasize=(tmsize_t)sbc[0];
			return;
		}
#endif
#ifdef OJPEG_SUPPORT
		if(t2p->tiff_compression == COMPRESSION_OJPEG){
			if(!TIFFGetField(input, TIFFTAG_STRIPBYTECOUNTS, &sbc)){
				TIFFError(TIFF2PDF_MODULE,
					"Input file %s missing field: TIFFTAG_STRIPBYTECOUNTS",
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				return;
			}
			stripcount=TIFFNumberOfStrips(input);
			for(i=0;i<stripcount;i++){
				k += sbc[i];
			}
			if(TIFFGetField(input, TIFFTAG_JPEGIFOFFSET, &(t2p->tiff_dataoffset))){
				if(t2p->tiff_dataoffset != 0){
					if(TIFFGetField(input, TIFFTAG_JPEGIFBYTECOUNT, &(t2p->tiff_datasize))!=0){
						if((uint64)t2p->tiff_datasize < k) {
							TIFFWarning(TIFF2PDF_MODULE,
								"Input file %s has short JPEG interchange file byte count",
								TIFFFileName(input));
								t2p->pdf_ojpegiflength=t2p->tiff_datasize;
								k += t2p->tiff_datasize;
								k += 6;
								k += stripcount;
								k += stripcount;
								t2p->tiff_datasize = (tsize_t) k;
								return;
							}
							return;
					}else {
						TIFFError(TIFF2PDF_MODULE,
							"Input file %s missing field: TIFFTAG_JPEGIFBYTECOUNT",
							TIFFFileName(input));
							t2p->t2p_error = T2P_ERR_ERROR;
							return;
					}
				}
			}
			k += stripcount;
			k += stripcount;
			k += 2048;
			t2p->tiff_datasize = (tsize_t) k;
			return;
		}
#endif
#ifdef JPEG_SUPPORT
		if(t2p->tiff_compression == COMPRESSION_JPEG) {
			uint32 count = 0;
			if(TIFFGetField(input, TIFFTAG_JPEGTABLES, &count, &jpt) != 0 ){
				if(count > 4){
					k += count;
					k -= 2;
				}
			} else {
				k = 2;
			}
			stripcount=TIFFNumberOfStrips(input);
			if(!TIFFGetField(input, TIFFTAG_STRIPBYTECOUNTS, &sbc)){
				TIFFError(TIFF2PDF_MODULE,
					"Input file %s missing field: TIFFTAG_STRIPBYTECOUNTS",
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				return;
			}
			for(i=0;i<stripcount;i++){
				k += sbc[i];
			}
			k += 2;
			k += 6;
			t2p->tiff_datasize = (tsize_t) k;
			return;
		}
#endif
		(void) 0;
	}
	k = TIFFScanlineSize(input) * t2p->tiff_length;
	if(t2p->tiff_planar==PLANARCONFIG_SEPARATE){
		k *= t2p->tiff_samplesperpixel;
	}
	if (k == 0) {
		t2p->t2p_error = T2P_ERR_ERROR;
	}
	t2p->tiff_datasize = (tsize_t) k;
	return;
}