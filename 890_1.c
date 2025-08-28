void t2p_tile_collapse_left(
	tdata_t buffer,
	tsize_t scanwidth,
	uint32 tilewidth,
	uint32 edgetilewidth,
	uint32 tilelength){
	uint32 i=0;
	tsize_t edgescanwidth=0;
	edgescanwidth = (scanwidth * edgetilewidth + (tilewidth - 1))/ tilewidth;
	for(i=i;i<tilelength;i++){
		_TIFFmemcpy(
			&(((char*)buffer)[edgescanwidth*i]),
			&(((char*)buffer)[scanwidth*i]),
			edgescanwidth);
	}
	return;
}