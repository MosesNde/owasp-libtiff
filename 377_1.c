static void
JPEGFixupTagsSubsampling(TIFF* tif)
{
	struct JPEGFixupTagsSubsamplingData m;
        if( tif->tif_dir.td_stripbytecount == NULL
            || tif->tif_dir.td_stripbytecount[0] == 0 )
        {
            return;
        }
	m.tif=tif;
	m.buffersize=2048;
	m.buffer=_TIFFmalloc(m.buffersize);
	if (m.buffer==NULL)
	{
		return;
	}
	m.buffercurrentbyte=NULL;
	m.bufferbytesleft=0;
	m.fileoffset=tif->tif_dir.td_stripoffset[0];
	m.filepositioned=0;
	m.filebytesleft=tif->tif_dir.td_stripbytecount[0];
	JPEGFixupTagsSubsamplingSec(&m);
	_TIFFfree(m.buffer);
}