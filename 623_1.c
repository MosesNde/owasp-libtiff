static int
TIFFLinkDirectory(TIFF* tif)
{
	static const char module[] = "TIFFLinkDirectory";
	tif->tif_diroff = (TIFFSeekFile(tif,0,SEEK_END)+1) &~ 1;
	if (tif->tif_flags & TIFF_INSUBIFD)
	{
		if (!(tif->tif_flags&TIFF_BIGTIFF))
		{
			uint32 m;
			m = (uint32)tif->tif_diroff;
			if (tif->tif_flags & TIFF_SWAB)
				TIFFSwabLong(&m);
			(void) TIFFSeekFile(tif, tif->tif_subifdoff, SEEK_SET);
			if (!WriteOK(tif, &m, 4)) {
				TIFFErrorExt(tif->tif_clientdata, module,
				     "Error writing SubIFD directory link");
				return (0);
			}
			if (--tif->tif_nsubifd)
				tif->tif_subifdoff += 4;
			else
				tif->tif_flags &= ~TIFF_INSUBIFD;
			return (1);
		}
		else
		{
			uint64 m;
			m = tif->tif_diroff;
			if (tif->tif_flags & TIFF_SWAB)
				TIFFSwabLong8(&m);
			(void) TIFFSeekFile(tif, tif->tif_subifdoff, SEEK_SET);
			if (!WriteOK(tif, &m, 8)) {
				TIFFErrorExt(tif->tif_clientdata, module,
				     "Error writing SubIFD directory link");
				return (0);
			}
			if (--tif->tif_nsubifd)
				tif->tif_subifdoff += 8;
			else
				tif->tif_flags &= ~TIFF_INSUBIFD;
			return (1);
		}
	}
	if (!(tif->tif_flags&TIFF_BIGTIFF))
	{
		uint32 m;
		uint32 nextdir;
		m = (uint32)(tif->tif_diroff);
		if (tif->tif_flags & TIFF_SWAB)
			TIFFSwabLong(&m);
		if (tif->tif_header.classic.tiff_diroff == 0) {
			tif->tif_header.classic.tiff_diroff = (uint32) tif->tif_diroff;
			(void) TIFFSeekFile(tif,4, SEEK_SET);
			if (!WriteOK(tif, &m, 4)) {
				TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
				     "Error writing TIFF header");
				return (0);
			}
			return (1);
		}
		nextdir = tif->tif_header.classic.tiff_diroff;
		while(1) {
			uint16 dircount;
			uint32 nextnextdir;
			if (!SeekOK(tif, nextdir) ||
			    !ReadOK(tif, &dircount, 2)) {
				TIFFErrorExt(tif->tif_clientdata, module,
				     "Error fetching directory count");
				return (0);
			}
			if (tif->tif_flags & TIFF_SWAB)
				TIFFSwabShort(&dircount);
			(void) TIFFSeekFile(tif,
			    nextdir+2+dircount*12, SEEK_SET);
			if (!ReadOK(tif, &nextnextdir, 4)) {
				TIFFErrorExt(tif->tif_clientdata, module,
				     "Error fetching directory link");
				return (0);
			}
			if (tif->tif_flags & TIFF_SWAB)
				TIFFSwabLong(&nextnextdir);
			if (nextnextdir==0)
			{
				(void) TIFFSeekFile(tif,
				    nextdir+2+dircount*12, SEEK_SET);
				if (!WriteOK(tif, &m, 4)) {
					TIFFErrorExt(tif->tif_clientdata, module,
				     "Error writing directory link");
					return (0);
				}
				break;
			}
			nextdir=nextnextdir;
		}
	}
	else
	{
		uint64 m;
		uint64 nextdir;
		m = tif->tif_diroff;
		if (tif->tif_flags & TIFF_SWAB)
			TIFFSwabLong8(&m);
		if (tif->tif_header.big.tiff_diroff == 0) {
			tif->tif_header.big.tiff_diroff = tif->tif_diroff;
			(void) TIFFSeekFile(tif,8, SEEK_SET);
			if (!WriteOK(tif, &m, 8)) {
				TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
				     "Error writing TIFF header");
				return (0);
			}
			return (1);
		}
		nextdir = tif->tif_header.big.tiff_diroff;
		while(1) {
			uint64 dircount64;
			uint16 dircount;
			uint64 nextnextdir;
			if (!SeekOK(tif, nextdir) ||
			    !ReadOK(tif, &dircount64, 8)) {
				TIFFErrorExt(tif->tif_clientdata, module,
				     "Error fetching directory count");
				return (0);
			}
			if (tif->tif_flags & TIFF_SWAB)
				TIFFSwabLong8(&dircount64);
			if (dircount64>0xFFFF)
			{
				TIFFErrorExt(tif->tif_clientdata, module,
				     "Sanity check on tag count failed, likely corrupt TIFF");
				return (0);
			}
			dircount=(uint16)dircount64;
			(void) TIFFSeekFile(tif,
			    nextdir+8+dircount*20, SEEK_SET);
			if (!ReadOK(tif, &nextnextdir, 8)) {
				TIFFErrorExt(tif->tif_clientdata, module,
				     "Error fetching directory link");
				return (0);
			}
			if (tif->tif_flags & TIFF_SWAB)
				TIFFSwabLong8(&nextnextdir);
			if (nextnextdir==0)
			{
				(void) TIFFSeekFile(tif,
				    nextdir+8+dircount*20, SEEK_SET);
				if (!WriteOK(tif, &m, 8)) {
					TIFFErrorExt(tif->tif_clientdata, module,
				     "Error writing directory link");
					return (0);
				}
				break;
			}
			nextdir=nextnextdir;
		}
	}
	return (1);
}