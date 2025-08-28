static int
TIFFFillStripPartial( TIFF *tif, int strip, int read_ahead, int restart )
{
	static const char module[] = "TIFFFillStripPartial";
	register TIFFDirectory *td = &tif->tif_dir;
	uint64 unused_data;
	uint64 read_offset;
	tmsize_t cc, to_read;
	tmsize_t bytecountm;
	bytecountm=(tmsize_t) td->td_stripbytecount[strip];
	if (read_ahead*2 > tif->tif_rawdatasize) {
		assert( restart );
		tif->tif_curstrip = NOSTRIP;
		if ((tif->tif_flags & TIFF_MYBUFFER) == 0) {
			TIFFErrorExt(tif->tif_clientdata, module,
					 "Data buffer too small to hold part of strip %lu",
					 (unsigned long) strip);
			return (0);
		}
		if (!TIFFReadBufferSetup(tif, 0, read_ahead*2))
			return (0);
	}
	if( restart )
	{
		tif->tif_rawdataloaded = 0;
		tif->tif_rawdataoff = 0;
	}
	if( tif->tif_rawdataloaded > 0 )
		unused_data = tif->tif_rawdataloaded - (tif->tif_rawcp - tif->tif_rawdata);
	else
		unused_data = 0;
	if( unused_data > 0 )
	{
		memmove( tif->tif_rawdata, tif->tif_rawcp, unused_data );
	}
	read_offset = td->td_stripoffset[strip]
			+ tif->tif_rawdataoff + tif->tif_rawdataloaded;
	if (!SeekOK(tif, read_offset)) {
		TIFFErrorExt(tif->tif_clientdata, module,
				 "Seek error at scanline %lu, strip %lu",
				 (unsigned long) tif->tif_row, (unsigned long) strip);
		return 0;
	}
	to_read = tif->tif_rawdatasize - unused_data;
	if( (uint64) to_read > td->td_stripbytecount[strip]
		- tif->tif_rawdataoff - tif->tif_rawdataloaded )
	{
		to_read = td->td_stripbytecount[strip]
				- tif->tif_rawdataoff - tif->tif_rawdataloaded;
	}
	cc = TIFFReadFile(tif, tif->tif_rawdata + unused_data, to_read);
	if (cc != to_read) {
#if defined(__WIN32__) && defined(_MSC_VER)
		TIFFErrorExt(tif->tif_clientdata, module,
				 "Read error at scanline %lu; got %I64u bytes, expected %I64u",
				 (unsigned long) tif->tif_row,
				 (unsigned __int64) cc,
				 (unsigned __int64) to_read);
#else
		TIFFErrorExt(tif->tif_clientdata, module,
				 "Read error at scanline %lu; got %llu bytes, expected %llu",
				 (unsigned long) tif->tif_row,
				 (unsigned long long) cc,
				 (unsigned long long) to_read);
#endif
		return 0;
	}
	tif->tif_rawdataoff = tif->tif_rawdataoff + tif->tif_rawdataloaded - unused_data ;
	tif->tif_rawdataloaded = unused_data + to_read;
	tif->tif_rawcp = tif->tif_rawdata;
	if (!isFillOrder(tif, td->td_fillorder) &&
		(tif->tif_flags & TIFF_NOBITREV) == 0)
		TIFFReverseBits(tif->tif_rawdata + unused_data, to_read );
	if( restart )
		return TIFFStartStrip(tif, strip);
	else
		return 1;
}