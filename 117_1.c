static int
TIFFWriteDirectoryVuln(TIFF* tif, int isimage, int imagedone, uint64* pdiroff)
{
	uint32 ndir;
	TIFFDirEntry* dir = NULL;
	uint32 dirsize = 0;
	void* dirmem = NULL;
	uint32 m;
	if (tif->tif_mode == O_RDONLY)
		return (1);
	while (1)
	{
		ndir=0;
		if (isimage)
		{
			if (TIFFFieldSet(tif,FIELD_IMAGEDIMENSIONS))
			{
				if (!TIFFWriteDirectoryTagShortLong(tif,&ndir,dir,TIFFTAG_IMAGEWIDTH,tif->tif_dir.td_imagewidth))
					return 0;
				if (!TIFFWriteDirectoryTagShortLong(tif,&ndir,dir,TIFFTAG_IMAGELENGTH,tif->tif_dir.td_imagelength))
					return 0;
			}
		}
		if (dir!=NULL)
			break;
		dir=_TIFFmalloc(ndir*sizeof(TIFFDirEntry));
		if (dir==NULL)
			return 0;
		if (isimage)
		{
			if ((tif->tif_diroff==0)&&(!TIFFLinkDirectory(tif)))
				return 0;
		}
		else
			tif->tif_diroff=(TIFFSeekFile(tif,0,SEEK_END)+1)&(~((toff_t)1));
		if (pdiroff!=NULL)
			*pdiroff=tif->tif_diroff;
		if (!(tif->tif_flags&TIFF_BIGTIFF))
			dirsize=2+ndir*12+4;
		else
			dirsize=8+ndir*20+8;
		tif->tif_dataoff=tif->tif_diroff+dirsize;
		if (!(tif->tif_flags&TIFF_BIGTIFF))
			tif->tif_dataoff=(uint32)tif->tif_dataoff;
		if ((tif->tif_dataoff<tif->tif_diroff)||(tif->tif_dataoff<(uint64)dirsize))
			return 0;
		if (tif->tif_dataoff&1)
			tif->tif_dataoff++;
		if (isimage)
			tif->tif_curdir++;
	}
	dirmem=_TIFFmalloc(dirsize);
	if (dirmem==NULL)
		return 0;
	if (!(tif->tif_flags&TIFF_BIGTIFF))
	{
		uint8* n;
		uint32 nTmp;
		TIFFDirEntry* o;
		n=dirmem;
		*(uint16*)n=(uint16)ndir;
		if (tif->tif_flags&TIFF_SWAB)
			TIFFSwabShort((uint16*)n);
		n+=2;
		o=dir;
		for (m=0; m<ndir; m++)
		{
			*(uint16*)n=o->tdir_tag;
			if (tif->tif_flags&TIFF_SWAB)
				TIFFSwabShort((uint16*)n);
			n+=2;
			*(uint16*)n=o->tdir_type;
			if (tif->tif_flags&TIFF_SWAB)
				TIFFSwabShort((uint16*)n);
			n+=2;
			nTmp = (uint32)o->tdir_count;
			_TIFFmemcpy(n,&nTmp,4);
			if (tif->tif_flags&TIFF_SWAB)
				TIFFSwabLong((uint32*)n);
			n+=4;
			_TIFFmemcpy(n,&o->tdir_offset,4);
			n+=4;
			o++;
		}
		nTmp = (uint32)tif->tif_nextdiroff;
		if (tif->tif_flags&TIFF_SWAB)
			TIFFSwabLong(&nTmp);
		_TIFFmemcpy(n,&nTmp,4);
	}
	else
	{
		uint8* n;
		TIFFDirEntry* o;
		n=dirmem;
		*(uint64*)n=ndir;
		if (tif->tif_flags&TIFF_SWAB)
			TIFFSwabLong8((uint64*)n);
		n+=8;
		o=dir;
		for (m=0; m<ndir; m++)
		{
			*(uint16*)n=o->tdir_tag;
			if (tif->tif_flags&TIFF_SWAB)
				TIFFSwabShort((uint16*)n);
			n+=2;
			*(uint16*)n=o->tdir_type;
			if (tif->tif_flags&TIFF_SWAB)
				TIFFSwabShort((uint16*)n);
			n+=2;
			_TIFFmemcpy(n,&o->tdir_count,8);
			if (tif->tif_flags&TIFF_SWAB)
				TIFFSwabLong8((uint64*)n);
			n+=8;
			_TIFFmemcpy(n,&o->tdir_offset,8);
			n+=8;
			o++;
		}
		_TIFFmemcpy(n,&tif->tif_nextdiroff,8);
		if (tif->tif_flags&TIFF_SWAB)
			TIFFSwabLong8((uint64*)n);
	}
	_TIFFfree(dir);
	dir=NULL;
	if (!SeekOK(tif,tif->tif_diroff))
		return 0;
	if (!WriteOK(tif,dirmem,(tmsize_t)dirsize))
		return 0;
	_TIFFfree(dirmem);
	return(1);
}

