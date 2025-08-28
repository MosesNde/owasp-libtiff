static tmsize_t
TIFFReadRawStrip1(TIFF* tif, uint32 strip, void* buf, tmsize_t size,
    const char* module)
{
	TIFFDirectory *td = &tif->tif_dir;
	if (!isMapped(tif)) {
		tmsize_t cc;
		cc = TIFFReadFile(tif, buf, size);
		if (cc != size) {
			return ((tmsize_t)(-1));
		}
	} else {
		tmsize_t ma,mb;
		tmsize_t n;
		ma=(tmsize_t)td->td_stripoffset[strip];
		mb=ma+size;
		n=size;
		_TIFFmemcpy(buf, tif->tif_base + ma,
			    size);
	}
	return (size);
}