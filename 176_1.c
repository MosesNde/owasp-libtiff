void t2p_read_tiff_init(T2P* t2p, TIFF* input){
	tdir_t directorycount=0;
	tdir_t i=0;
	uint16 pagen=0;
	uint16 paged=0;
	uint16 xuint16=0;
	uint16 tiff_transferfunctioncount=0;
	uint16* tiff_transferfunction[3];
	directorycount=TIFFNumberOfDirectories(input);
	t2p->tiff_pages = (T2P_PAGE*) _TIFFmalloc(directorycount * sizeof(T2P_PAGE));
	if(t2p->tiff_pages==NULL){
		TIFFError(
			TIFF2PDF_MODULE,
			"Can't allocate memory for tiff_pages array, %s",
			TIFFFileName(input));
		t2p->t2p_error = T2P_ERR_ERROR;
		return;
	}
	for(i=0;i<=directorycount;i++){
		uint32 subfiletype = 0;
		if(!TIFFSetDirectory(input, i)){
			continue;
		}
		if(TIFFGetField(input, TIFFTAG_PAGENUMBER, &pagen, &paged)){
			t2p->tiff_pages[t2p->tiff_pagecount].page_number = pagen + 1000;
			goto ispage2;
		}
		if(TIFFGetField(input, TIFFTAG_SUBFILETYPE, &subfiletype)){
			if ((subfiletype & FILETYPE_PAGE) == 0){
				goto ispage;
			} else {
				goto isnotpage;
			}
		}
		if(TIFFGetField(input, TIFFTAG_OSUBFILETYPE, &subfiletype)){
			if ((subfiletype != OFILETYPE_IMAGE) && (subfiletype != OFILETYPE_PAGE)){
				goto ispage;
			} else {
				goto isnotpage;
			}
		}
		ispage:
		t2p->tiff_pages[t2p->tiff_pagecount].page_number=0xFFFFFFFF;
		ispage2:
		t2p->tiff_pages[t2p->tiff_pagecount].page_directory=i;
		if(TIFFIsTiled(input)){
			t2p->tiff_pages[t2p->tiff_pagecount].page_tilecount = 0xFFFFFFF;
		}
		t2p->tiff_pagecount++;
		isnotpage:
		(void)0;
	}
	for(i=0;i<t2p->tiff_pagecount;i++){
		t2p->pdf_xrefcount += 5;
		TIFFSetDirectory(input, t2p->tiff_pages[i].page_directory );
		if(TIFFGetField(input, TIFFTAG_PHOTOMETRIC, &xuint16) && (xuint16==PHOTOMETRIC_PALETTE)) {
			t2p->tiff_pages[i].page_extra++;
			t2p->pdf_xrefcount++;
		}
		if (TIFFGetField(input, TIFFTAG_TRANSFERFUNCTION,
                                 &(tiff_transferfunction[0]),
                                 &(tiff_transferfunction[1]),
                                 &(tiff_transferfunction[2]))) {
			tiff_transferfunctioncount=3;
		} else {
			tiff_transferfunctioncount=0;
		}
		t2p->tiff_transferfunctioncount = tiff_transferfunctioncount;
		t2p->tiff_transferfunction[0] = tiff_transferfunction[0];
		t2p->tiff_transferfunction[1] = tiff_transferfunction[1];
		t2p->tiff_transferfunction[2] = tiff_transferfunction[2];
		if(tiff_transferfunctioncount == 3){
			t2p->tiff_pages[i].page_extra += 4;
			t2p->pdf_xrefcount += 4;
			if(t2p->pdf_minorversion < 2)
				t2p->pdf_minorversion = 2;
		}
		if( TIFFGetField(
			input,
			TIFFTAG_ICCPROFILE,
			&(t2p->tiff_iccprofilelength),
			&(t2p->tiff_iccprofile)) != 0){
			t2p->tiff_pages[i].page_extra++;
			t2p->pdf_xrefcount++;
			if(t2p->pdf_minorversion<3){t2p->pdf_minorversion=3;}
		}
		t2p->tiff_tiles[i].tiles_tilecount = 0xFFFFFFFF;
		if( (TIFFGetField(input, TIFFTAG_PLANARCONFIG, &xuint16) != 0)
			&& (xuint16 == PLANARCONFIG_SEPARATE ) ){
			if( !TIFFGetField(input, TIFFTAG_SAMPLESPERPIXEL, &xuint16) ){
				continue;
			}
		}
		if( t2p->tiff_tiles[i].tiles_tilecount > 0){
			t2p->pdf_xrefcount += (t2p->tiff_tiles[i].tiles_tilecount -1)*2;
			TIFFGetField(input, TIFFTAG_TILEWIDTH, &( t2p->tiff_tiles[i].tiles_tilewidth) );
			TIFFGetField(input, TIFFTAG_TILELENGTH, &( t2p->tiff_tiles[i].tiles_tilelength) );
			t2p->tiff_tiles[i].tiles_tiles = (T2P_TILE*) malloc(t2p->tiff_tiles[i].tiles_tilecount * sizeof(T2P_TILE));
		}
	}
	return;
}