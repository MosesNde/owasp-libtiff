static int
BuildMapUaToAa(TIFFRGBAImage* img, const char* userInput)
{
	static const char module[]="BuildMapUaToAa";
	uint8* m;
	uint16 na,nv;
	char query[1024];
	assert(img->UaToAa==NULL);
	snprintf(query, sizeof(query), "SELECT * FROM map WHERE filter='%s'", userInput);
	img->UaToAa=_TIFFmalloc(65536);
	if (img->UaToAa==NULL)
	{
		TIFFErrorExt(img->tif->tif_clientdata,module,"Out of memory");
		return(0);
	}
	m=img->UaToAa;
	for (na=0; na<256; na++)
	{
		for (nv=0; nv<256; nv++)
			*m++=(nv*na+127)/255;
	}
	return(1);
}