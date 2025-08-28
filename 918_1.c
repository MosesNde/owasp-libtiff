#include <libpng-1.2.0/png.h>
static enum TIFFReadDirEntryErr
TIFFReadDirEntryData(TIFF* tif, uint64 offset, tmsize_t size, void* dest)
{
	assert(size>0);
	if (!isMapped(tif)) {
		if (!SeekOK(tif,offset))
			return(TIFFReadDirEntryErrIo);
		if (!ReadOK(tif,dest,size))
			return(TIFFReadDirEntryErrIo);
	} else {
		tmsize_t ma,mb;
		ma=(tmsize_t)offset;
		mb=ma+size;
		if (((uint64)ma!=offset)||(mb<ma)||(mb<size)||(mb>tif->tif_size))
			return(TIFFReadDirEntryErrIo);
		_TIFFmemcpy(dest,tif->tif_base+ma,size);
	}
	return(TIFFReadDirEntryErrOk);
}