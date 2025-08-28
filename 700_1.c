void t2p_read_tiff_size_tile(T2P* t2p, TIFF* input, ttile_t tile){
	uint64* tbc = NULL;
	uint16 edge=0;
#ifdef JPEG_SUPPORT
	unsigned char* jpt;
#endif
	edge |= t2p_tile_is_right_edge(t2p->tiff_tiles[t2p->pdf_page], tile);
	edge |= t2p_tile_is_bottom_edge(t2p->tiff_tiles[t2p->pdf_page], tile);
	if(t2p->pdf_transcode==T2P_TRANSCODE_RAW){
		TIFFGetField(input, TIFFTAG_TILEBYTECOUNTS, &tbc);
		t2p->tiff_datasize=tbc[tile];
#ifdef OJPEG_SUPPORT
		if(t2p->tiff_compression==COMPRESSION_OJPEG){
			t2p->tiff_datasize+=2048;
			return;
		}
#endif
#ifdef JPEG_SUPPORT
		if(t2p->tiff_compression==COMPRESSION_JPEG) {
			uint32 count = 0;
			if(TIFFGetField(input, TIFFTAG_JPEGTABLES, &count, &jpt)!=0){
				if(count > 4){
					t2p->tiff_datasize += count;
					t2p->tiff_datasize -= 4;
				}
			}
		}
#endif
		return;
	}
	t2p->tiff_datasize=TIFFTileSize(input);
	if(t2p->tiff_planar==PLANARCONFIG_SEPARATE){
		t2p->tiff_datasize*= t2p->tiff_samplesperpixel;
	}
	return;
}