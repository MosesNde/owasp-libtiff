static int
TIFFWriteDirectoryTagShortPerSample(TIFF* tif, uint32* ndir, TIFFDirEntry* dir, uint16 tag, uint16 value)
{
	static const char module[] = "TIFFWriteDirectoryTagShortPerSample";
	uint16* m;
	uint16* na;
	uint16 nb;
	int o;
	if (dir==NULL)
	{
		(*ndir)++;
		return(1);
	}
	m=malloc(tif->tif_dir.td_samplesperpixel*sizeof(uint16));
	if (m==NULL)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
		return(0);
	}
	for (na=m, nb=0; nb<tif->tif_dir.td_samplesperpixel; na++, nb++)
		*na=value;
	o=TIFFWriteDirectoryTagCheckedShortArray(tif,ndir,dir,tag,tif->tif_dir.td_samplesperpixel,m);
	free(m);
	return(o);
}