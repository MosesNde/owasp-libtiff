static enum TIFFReadDirEntryErr TIFFReadDirEntryPersampleDouble(TIFF* tif, TIFFDirEntry* direntry, double* value)
{
	enum TIFFReadDirEntryErr err;
	double* m;
	uint16 nb;
	if (direntry->tdir_count!=(uint64)tif->tif_dir.td_samplesperpixel)
		return(TIFFReadDirEntryErrCount);
	err=TIFFReadDirEntryDoubleArray(tif,direntry,&m);
	if (err!=TIFFReadDirEntryErrOk)
		return(err);
	*value=*m;
	_TIFFfree(m);
	return(TIFFReadDirEntryErrOk);
}