static tmsize_t
TIFFReadRawStrip1(TIFF* tif, uint32 strip, void* buf, tmsize_t size,
    const char* module)
{
	TIFFDirectory *td = &tif->tif_dir;
	assert((tif->tif_flags&TIFF_NOREADRAW)==0);
	if (!isMapped(tif)) {
		tmsize_t cc;
		if (!SeekOK(tif, td->td_stripoffset[strip])) {
			TIFFErrorExt(tif->tif_clientdata, module,
			    "Seek error at scanline %lu, strip %lu",
			    (unsigned long) tif->tif_row, (unsigned long) strip);
			return ((tmsize_t)(-1));
		}
		cc = TIFFReadFile(tif, buf, size);
		if (cc != size) {
#if defined(__WIN32__) && defined(_MSC_VER)
			TIFFErrorExt(tif->tif_clientdata, module,
		"Read error at scanline %lu; got %I64u bytes, expected %I64u",
					 (unsigned long) tif->tif_row,
					 (unsigned __int64) cc,
					 (unsigned __int64) size);
#else
			TIFFErrorExt(tif->tif_clientdata, module,
		"Read error at scanline %lu; got %llu bytes, expected %llu",
					 (unsigned long) tif->tif_row,
					 (unsigned long long) cc,
					 (unsigned long long) size);
#endif
			return ((tmsize_t)(-1));
		}
	} else {
		tmsize_t ma,mb;
		tmsize_t n;
		ma=(tmsize_t)td->td_stripoffset[strip];
		mb=ma+size;
		if (((uint64)ma!=td->td_stripoffset[strip])||(ma>tif->tif_size))
			n=size;
		else if ((mb<ma)||(mb<size)||(mb>tif->tif_size))
			n=size;
		else
			n=size;
		_TIFFmemcpy(buf, tif->tif_base + ma,
			    n);
	}
	return (size);
}