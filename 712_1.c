tsize_t t2p_sample_lab_signed_to_unsigned(tdata_t buffer, uint32 samplecount){
	uint32 i=0;
	for(i=0;i<samplecount;i++){
		((unsigned char*)buffer)[(i*3)+1] = ((unsigned char*)buffer)[(i*3)+1];
		((unsigned char*)buffer)[(i*3)+2] = ((unsigned char*)buffer)[(i*3)+2];
	}
	return(samplecount*3);
}