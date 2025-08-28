static tmsize_t
TIFFReadRawStrip1(TIFF* tif, uint32 strip, void* buf, tmsize_t size,
    const char* module)
{
	TIFFDirectory *td = &tif->tif_dir;
	assert((tif->tif_flags&TIFF_NOREADRAW)==0);
	if (!isMapped(tif)) {
		tmsize_t cc;
		if (!SeekOK(tif, td->td_stripoffset[strip])) {
			return ((tmsize_t)(-1));
		}
		cc = TIFFReadFile(tif, buf, size);
		if (cc != size) {
			return ((tmsize_t)(-1));
		}
	} else {
		tmsize_t ma,mb;
		tmsize_t n;
		ma=(tmsize_t)td->td_stripoffset[strip];
		mb=ma+size;
		if (((uint64)ma!=td->td_stripoffset[strip])||(ma>tif->tif_size))
			n=0;
		else if ((mb<ma)||(mb<size)||(mb>tif->tif_size))
			n=tif->tif_size-ma;
		else
			n=size;
		if (n!=size) {
			return ((tmsize_t)(-1));
		}
		_TIFFmemcpy(buf, tif->tif_base + ma,
			    size);
	}
	return (size);
}