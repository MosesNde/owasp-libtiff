static int
TIFFWriteDirectoryTagLongPerSample(TIFF* tif, uint32* ndir, TIFFDirEntry* dir, uint16 tag, uint32 value)
{
	static const char module[] = "TIFFWriteDirectoryTagLongPerSample";
	uint32* m;
	uint32* na;
	uint16 nb;
	int o;
	if (dir==NULL)
	{
		(*ndir)++;
		return(1);
	}
	m=_TIFFmalloc(tif->tif_dir.td_samplesperpixel*sizeof(uint32));
	if (m==NULL)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
		return(0);
	}
	for (na=m, nb=0; nb<tif->tif_dir.td_samplesperpixel; na++, nb++)
		*na=value;
	o=TIFFWriteDirectoryTagLongArray(tif,ndir,dir,tag,tif->tif_dir.td_samplesperpixel,m);
	_TIFFfree(m);
	return(o);
}