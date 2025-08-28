static enum TIFFReadDirEntryErr TIFFReadDirEntryPersampleDouble(TIFF* tif, TIFFDirEntry* direntry, double* value)
{
	enum TIFFReadDirEntryErr err;
	double* m;
	double* na;
	uint16 nb;
	if (direntry->tdir_count!=(uint64)tif->tif_dir.td_samplesperpixel)
		return(TIFFReadDirEntryErrCount);
	err=TIFFReadDirEntryDoubleArray(tif,direntry,&m);
	if (err!=TIFFReadDirEntryErrOk)
		return(err);
	nb=tif->tif_dir.td_samplesperpixel;
	na=m;
	*value=*na++;
	nb--;
	while (nb>0)
	{
		*value=*na++;
		nb--;
	}
	_TIFFfree(m);
	return(TIFFReadDirEntryErrOk);
}