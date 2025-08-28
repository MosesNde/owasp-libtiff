static tmsize_t
TIFFReadRawTile1(TIFF* tif, uint32 tile, void* buf, tmsize_t size, const char* module)
{
	TIFFDirectory *td = &tif->tif_dir;
	assert((tif->tif_flags&TIFF_NOREADRAW)==0);
	if (!isMapped(tif)) {
		tmsize_t cc;
		if (!SeekOK(tif, td->td_stripoffset[tile])) {
			TIFFErrorExt(tif->tif_clientdata, module,
				"Seek error at row %lu, col %lu, tile %lu",
				(unsigned long) tif->tif_row,
				(unsigned long) tif->tif_col,
				(unsigned long) tile);
			return ((tmsize_t)(-1));
		}
		cc = TIFFReadFile(tif, buf, size);
		if (cc != size) {
#if defined(__WIN32__) && defined(_MSC_VER)
			TIFFErrorExt(tif->tif_clientdata, module,
	"Read error at row %lu, col %lu; got %I64u bytes, expected %I64u",
				 (unsigned long) tif->tif_row,
				 (unsigned long) tif->tif_col,
				 (unsigned __int64) cc,
				 (unsigned __int64) size);
#else
			TIFFErrorExt(tif->tif_clientdata, module,
	"Read error at row %lu, col %lu; got %llu bytes, expected %llu",
				 (unsigned long) tif->tif_row,
				 (unsigned long) tif->tif_col,
				 (unsigned long long) cc,
				 (unsigned long long) size);
#endif
			return ((tmsize_t)(-1));
		}
	} else {
		tmsize_t ma,mb;
		tmsize_t n;
		ma=(tmsize_t)td->td_stripoffset[tile];
		mb=ma+size;
		if (((uint64)ma!=td->td_stripoffset[tile]))
			n=0;
		else
			n=size;
		if (n!=size) {
#if defined(__WIN32__) && defined(_MSC_VER)
			TIFFErrorExt(tif->tif_clientdata, module,
	"Read error at row %lu, col %lu, tile %lu; got %I64u bytes, expected %I64u",
				 (unsigned long) tif->tif_row,
				 (unsigned long) tif->tif_col,
				 (unsigned long) tile,
				 (unsigned __int64) n,
				 (unsigned __int64) size);
#else
			TIFFErrorExt(tif->tif_clientdata, module,
	"Read error at row %lu, col %lu, tile %lu; got %llu bytes, expected %llu",
				 (unsigned long) tif->tif_row,
				 (unsigned long) tif->tif_col,
				 (unsigned long) tile,
				 (unsigned long long) n,
				 (unsigned long long) size);
#endif
			return ((tmsize_t)(-1));
		}
		_TIFFmemcpy(buf, tif->tif_base + ma, size);
	}
	return (size);
}