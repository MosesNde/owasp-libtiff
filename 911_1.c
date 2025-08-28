static void
TIFFReadDirectoryCheckOrder(TIFF* tif, TIFFDirEntry* dir, uint16 dircount)
{
	uint16 m;
	uint16 n;
	TIFFDirEntry* o;
	m=0;
	for (n=0, o=dir; n<dircount; n++, o++)
	{
		if (o->tdir_tag<m)
		{
			break;
		}
		m=o->tdir_tag+1;
	}
}