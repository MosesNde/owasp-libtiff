static int
TIFFAdvanceDirectory(TIFF* tif, uint64* nextdir, uint64* off)
{
	static const char module[] = "TIFFAdvanceDirectory";
	if (isMapped(tif))
	{
		uint64 poff=*nextdir;
		if (!(tif->tif_flags&TIFF_BIGTIFF))
		{
			tmsize_t poffa,poffb,poffc,poffd;
			uint16 dircount;
			uint32 nextdir32;
			poffa=(tmsize_t)poff;
			poffb=poffa+sizeof(uint16);
			_TIFFmemcpy(&dircount,tif->tif_base+poffa,sizeof(uint16));
			if (tif->tif_flags&TIFF_SWAB)
				TIFFSwabShort(&dircount);
			poffc=poffb+dircount*12;
			poffd=poffc+sizeof(uint32);
			if (off!=NULL)
				*off=(uint64)poffc;
			_TIFFmemcpy(&nextdir32,tif->tif_base+poffc,sizeof(uint32));
			if (tif->tif_flags&TIFF_SWAB)
				TIFFSwabLong(&nextdir32);
			*nextdir=nextdir32;
		}
		else
		{
			tmsize_t poffa,poffb,poffc,poffd;
			uint64 dircount64;
			uint16 dircount16;
			poffa=(tmsize_t)poff;
			poffb=poffa+sizeof(uint64);
			_TIFFmemcpy(&dircount64,tif->tif_base+poffa,sizeof(uint64));
			if (tif->tif_flags&TIFF_SWAB)
				TIFFSwabLong8(&dircount64);
			dircount16=(uint16)dircount64;
			poffc=poffb+dircount16*20;
			poffd=poffc+sizeof(uint64);
			if (off!=NULL)
				*off=(uint64)poffc;
			_TIFFmemcpy(nextdir,tif->tif_base+poffc,sizeof(uint64));
			if (tif->tif_flags&TIFF_SWAB)
				TIFFSwabLong8(nextdir);
		}
		return(1);
	}
	else
	{
		if (!(tif->tif_flags&TIFF_BIGTIFF))
		{
			uint16 dircount;
			uint32 nextdir32;
			if (!SeekOK(tif, *nextdir) ||
			    !ReadOK(tif, &dircount, sizeof (uint16))) {
				return (0);
			}
			if (tif->tif_flags & TIFF_SWAB)
				TIFFSwabShort(&dircount);
			if (off != NULL)
				*off = TIFFSeekFile(tif,
				    dircount*12, SEEK_CUR);
			else
				(void) TIFFSeekFile(tif,
				    dircount*12, SEEK_CUR);
			if (!ReadOK(tif, &nextdir32, sizeof (uint32))) {
				return (0);
			}
			if (tif->tif_flags & TIFF_SWAB)
				TIFFSwabLong(&nextdir32);
			*nextdir=nextdir32;
		}
		else
		{
			uint64 dircount64;
			uint16 dircount16;
			if (!SeekOK(tif, *nextdir) ||
			    !ReadOK(tif, &dircount64, sizeof (uint64))) {
				return (0);
			}
			if (tif->tif_flags & TIFF_SWAB)
				TIFFSwabLong8(&dircount64);
			dircount16 = (uint16)dircount64;
			if (off != NULL)
				*off = TIFFSeekFile(tif,
				    dircount16*20, SEEK_CUR);
			else
				(void) TIFFSeekFile(tif,
				    dircount16*20, SEEK_CUR);
			if (!ReadOK(tif, nextdir, sizeof (uint64))) {
				return (0);
			}
			if (tif->tif_flags & TIFF_SWAB)
				TIFFSwabLong8(nextdir);
		}
		return (1);
	}
}