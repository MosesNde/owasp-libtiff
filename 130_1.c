static int
TIFFWriteDirectorySec(TIFF* tif, int isimage, int imagedone, uint64* pdiroff)
{
 static const char module[] = "TIFFWriteDirectorySec";
 uint32 ndir;
 TIFFDirEntry* dir;
 uint32 dirsize;
 void* dirmem;
 uint32 m;
 if (tif->tif_mode == O_RDONLY)
  return (1);
 if (imagedone)
 {
  if (tif->tif_flags & TIFF_POSTENCODE)
  {
   tif->tif_flags &= ~TIFF_POSTENCODE;
   (*tif->tif_postencode)(tif);
  }
  (*tif->tif_close)(tif);
  if (tif->tif_rawcc > 0
      && (tif->tif_flags & TIFF_BEENWRITING) != 0 )
  {
      TIFFFlushData1(tif);
  }
  if ((tif->tif_flags & TIFF_MYBUFFER) && tif->tif_rawdata)
  {
   _TIFFfree(tif->tif_rawdata);
   tif->tif_rawdata = NULL;
   tif->tif_rawcc = 0;
   tif->tif_rawdatasize = 0;
   tif->tif_rawdataoff = 0;
   tif->tif_rawdataloaded = 0;
  }
  tif->tif_flags &= ~(TIFF_BEENWRITING|TIFF_BUFFERSETUP);
 }
 dir=NULL;
 dirmem=NULL;
 dirsize=0;
 while (1)
 {
  ndir=0;
  if (isimage)
  {
   if (TIFFFieldSet(tif,FIELD_IMAGEDIMENSIONS))
   {
    TIFFWriteDirectoryTagShortLong(tif,&ndir,dir,TIFFTAG_IMAGEWIDTH,tif->tif_dir.td_imagewidth);
    TIFFWriteDirectoryTagShortLong(tif,&ndir,dir,TIFFTAG_IMAGELENGTH,tif->tif_dir.td_imagelength);
   }
   if (TIFFFieldSet(tif,FIELD_TILEDIMENSIONS))
   {
    TIFFWriteDirectoryTagShortLong(tif,&ndir,dir,TIFFTAG_TILEWIDTH,tif->tif_dir.td_tilewidth);
    TIFFWriteDirectoryTagShortLong(tif,&ndir,dir,TIFFTAG_TILELENGTH,tif->tif_dir.td_tilelength);
   }
   if (TIFFFieldSet(tif,FIELD_RESOLUTION))
   {
    TIFFWriteDirectoryTagRational(tif,&ndir,dir,TIFFTAG_XRESOLUTION,tif->tif_dir.td_xresolution);
    TIFFWriteDirectoryTagRational(tif,&ndir,dir,TIFFTAG_YRESOLUTION,tif->tif_dir.td_yresolution);
   }
   if (TIFFFieldSet(tif,FIELD_POSITION))
   {
    TIFFWriteDirectoryTagRational(tif,&ndir,dir,TIFFTAG_XPOSITION,tif->tif_dir.td_xposition);
    TIFFWriteDirectoryTagRational(tif,&ndir,dir,TIFFTAG_YPOSITION,tif->tif_dir.td_yposition);
   }
   if (TIFFFieldSet(tif,FIELD_SUBFILETYPE))
   {
    TIFFWriteDirectoryTagLong(tif,&ndir,dir,TIFFTAG_SUBFILETYPE,tif->tif_dir.td_subfiletype);
   }
   if (TIFFFieldSet(tif,FIELD_BITSPERSAMPLE))
   {
    TIFFWriteDirectoryTagShortPerSample(tif,&ndir,dir,TIFFTAG_BITSPERSAMPLE,tif->tif_dir.td_bitspersample);
   }
   if (TIFFFieldSet(tif,FIELD_COMPRESSION))
   {
    TIFFWriteDirectoryTagShort(tif,&ndir,dir,TIFFTAG_COMPRESSION,tif->tif_dir.td_compression);
   }
   if (TIFFFieldSet(tif,FIELD_PHOTOMETRIC))
   {
    TIFFWriteDirectoryTagShort(tif,&ndir,dir,TIFFTAG_PHOTOMETRIC,tif->tif_dir.td_photometric);
   }
   if (TIFFFieldSet(tif,FIELD_THRESHHOLDING))
   {
    TIFFWriteDirectoryTagShort(tif,&ndir,dir,TIFFTAG_THRESHHOLDING,tif->tif_dir.td_threshholding);
   }
   if (TIFFFieldSet(tif,FIELD_FILLORDER))
   {
    TIFFWriteDirectoryTagShort(tif,&ndir,dir,TIFFTAG_FILLORDER,tif->tif_dir.td_fillorder);
   }
   if (TIFFFieldSet(tif,FIELD_ORIENTATION))
   {
    TIFFWriteDirectoryTagShort(tif,&ndir,dir,TIFFTAG_ORIENTATION,tif->tif_dir.td_orientation);
   }
   if (TIFFFieldSet(tif,FIELD_SAMPLESPERPIXEL))
   {
    TIFFWriteDirectoryTagShort(tif,&ndir,dir,TIFFTAG_SAMPLESPERPIXEL,tif->tif_dir.td_samplesperpixel);
   }
   if (TIFFFieldSet(tif,FIELD_ROWSPERSTRIP))
   {
    TIFFWriteDirectoryTagShortLong(tif,&ndir,dir,TIFFTAG_ROWSPERSTRIP,tif->tif_dir.td_rowsperstrip);
   }
   if (TIFFFieldSet(tif,FIELD_MINSAMPLEVALUE))
   {
    TIFFWriteDirectoryTagShortPerSample(tif,&ndir,dir,TIFFTAG_MINSAMPLEVALUE,tif->tif_dir.td_minsamplevalue);
   }
   if (TIFFFieldSet(tif,FIELD_MAXSAMPLEVALUE))
   {
    TIFFWriteDirectoryTagShortPerSample(tif,&ndir,dir,TIFFTAG_MAXSAMPLEVALUE,tif->tif_dir.td_maxsamplevalue);
   }
   if (TIFFFieldSet(tif,FIELD_PLANARCONFIG))
   {
    TIFFWriteDirectoryTagShort(tif,&ndir,dir,TIFFTAG_PLANARCONFIG,tif->tif_dir.td_planarconfig);
   }
   if (TIFFFieldSet(tif,FIELD_RESOLUTIONUNIT))
   {
    TIFFWriteDirectoryTagShort(tif,&ndir,dir,TIFFTAG_RESOLUTIONUNIT,tif->tif_dir.td_resolutionunit);
   }
   if (TIFFFieldSet(tif,FIELD_PAGENUMBER))
   {
    TIFFWriteDirectoryTagShortArray(tif,&ndir,dir,TIFFTAG_PAGENUMBER,2,&tif->tif_dir.td_pagenumber[0]);
   }
   if (TIFFFieldSet(tif,FIELD_STRIPBYTECOUNTS))
   {
    if (!isTiled(tif))
    {
     TIFFWriteDirectoryTagLongLong8Array(tif,&ndir,dir,TIFFTAG_STRIPBYTECOUNTS,tif->tif_dir.td_nstrips,tif->tif_dir.td_stripbytecount);
    }
    else
    {
     TIFFWriteDirectoryTagLongLong8Array(tif,&ndir,dir,TIFFTAG_TILEBYTECOUNTS,tif->tif_dir.td_nstrips,tif->tif_dir.td_stripbytecount);
    }
   }
   if (TIFFFieldSet(tif,FIELD_STRIPOFFSETS))
   {
    if (!isTiled(tif))
    {
     TIFFWriteDirectoryTagLongLong8Array(tif,&ndir,dir,TIFFTAG_STRIPOFFSETS,tif->tif_dir.td_nstrips,tif->tif_dir.td_stripoffset);
    }
    else
    {
     TIFFWriteDirectoryTagLongLong8Array(tif,&ndir,dir,TIFFTAG_TILEOFFSETS,tif->tif_dir.td_nstrips,tif->tif_dir.td_stripoffset);
    }
   }
   if (TIFFFieldSet(tif,FIELD_COLORMAP))
   {
    TIFFWriteDirectoryTagColormap(tif,&ndir,dir);
   }
   if (TIFFFieldSet(tif,FIELD_EXTRASAMPLES))
   {
    if (tif->tif_dir.td_extrasamples)
    {
     uint16 na;
     uint16* nb;
     TIFFGetFieldDefaulted(tif,TIFFTAG_EXTRASAMPLES,&na,&nb);
     TIFFWriteDirectoryTagShortArray(tif,&ndir,dir,TIFFTAG_EXTRASAMPLES,na,nb);
    }
   }
   if (TIFFFieldSet(tif,FIELD_SAMPLEFORMAT))
   {
    TIFFWriteDirectoryTagShortPerSample(tif,&ndir,dir,TIFFTAG_SAMPLEFORMAT,tif->tif_dir.td_sampleformat);
   }
   if (TIFFFieldSet(tif,FIELD_SMINSAMPLEVALUE))
   {
    TIFFWriteDirectoryTagSampleformatPerSample(tif,&ndir,dir,TIFFTAG_SMINSAMPLEVALUE,tif->tif_dir.td_sminsamplevalue);
   }
   if (TIFFFieldSet(tif,FIELD_SMAXSAMPLEVALUE))
   {
    TIFFWriteDirectoryTagSampleformatPerSample(tif,&ndir,dir,TIFFTAG_SMAXSAMPLEVALUE,tif->tif_dir.td_smaxsamplevalue);
   }
   if (TIFFFieldSet(tif,FIELD_IMAGEDEPTH))
   {
    TIFFWriteDirectoryTagLong(tif,&ndir,dir,TIFFTAG_IMAGEDEPTH,tif->tif_dir.td_imagedepth);
   }
   if (TIFFFieldSet(tif,FIELD_TILEDEPTH))
   {
    TIFFWriteDirectoryTagLong(tif,&ndir,dir,TIFFTAG_TILEDEPTH,tif->tif_dir.td_tiledepth);
   }
   if (TIFFFieldSet(tif,FIELD_HALFTONEHINTS))
   {
    TIFFWriteDirectoryTagShortArray(tif,&ndir,dir,TIFFTAG_HALFTONEHINTS,2,&tif->tif_dir.td_halftonehints[0]);
   }
   if (TIFFFieldSet(tif,FIELD_YCBCRSUBSAMPLING))
   {
    TIFFWriteDirectoryTagShortArray(tif,&ndir,dir,TIFFTAG_YCBCRSUBSAMPLING,2,&tif->tif_dir.td_ycbcrsubsampling[0]);
   }
   if (TIFFFieldSet(tif,FIELD_YCBCRPOSITIONING))
   {
    TIFFWriteDirectoryTagShort(tif,&ndir,dir,TIFFTAG_YCBCRPOSITIONING,tif->tif_dir.td_ycbcrpositioning);
   }
   if (TIFFFieldSet(tif,FIELD_TRANSFERFUNCTION))
   {
    TIFFWriteDirectoryTagTransferfunction(tif,&ndir,dir);
   }
   if (TIFFFieldSet(tif,FIELD_INKNAMES))
   {
    TIFFWriteDirectoryTagAscii(tif,&ndir,dir,TIFFTAG_INKNAMES,tif->tif_dir.td_inknameslen,tif->tif_dir.td_inknames);
   }
   if (TIFFFieldSet(tif,FIELD_SUBIFD))
   {
    TIFFWriteDirectoryTagSubifd(tif,&ndir,dir);
   }
   {
    uint32 n;
    for (n=0; n<tif->tif_nfields; n++) {
     const TIFFField* o;
     o = tif->tif_fields[n];
     if ((o->field_bit>=FIELD_CODEC)&&(TIFFFieldSet(tif,o->field_bit)))
     {
      switch (o->get_field_type)
      {
       case TIFF_SETGET_ASCII:
        {
         uint32 pa;
         char* pb;
         TIFFGetField(tif,o->field_tag,&pb);
         pa=(uint32)(strlen(pb));
         TIFFWriteDirectoryTagAscii(tif,&ndir,dir,o->field_tag,pa,pb);
        }
        break;
       case TIFF_SETGET_UINT16:
        {
         uint16 p;
         TIFFGetField(tif,o->field_tag,&p);
         TIFFWriteDirectoryTagShort(tif,&ndir,dir,o->field_tag,p);
        }
        break;
       case TIFF_SETGET_UINT32:
        {
         uint32 p;
         TIFFGetField(tif,o->field_tag,&p);
         TIFFWriteDirectoryTagLong(tif,&ndir,dir,o->field_tag,p);
        }
        break;
       case TIFF_SETGET_C32_UINT8:
        {
         uint32 pa;
         void* pb;
         TIFFGetField(tif,o->field_tag,&pa,&pb);
         TIFFWriteDirectoryTagUndefinedArray(tif,&ndir,dir,o->field_tag,pa,pb);
        }
        break;
       default:
        break;
      }
     }
    }
   }
  }
  for (m=0; m<(uint32)(tif->tif_dir.td_customValueCount); m++)
  {
   switch (tif->tif_dir.td_customValues[m].info->field_type)
   {
    case TIFF_ASCII:
     TIFFWriteDirectoryTagAscii(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_UNDEFINED:
     TIFFWriteDirectoryTagUndefinedArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_BYTE:
     TIFFWriteDirectoryTagByteArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_SBYTE:
     TIFFWriteDirectoryTagSbyteArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_SHORT:
     TIFFWriteDirectoryTagShortArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_SSHORT:
     TIFFWriteDirectoryTagSshortArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_LONG:
     TIFFWriteDirectoryTagLongArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_SLONG:
     TIFFWriteDirectoryTagSlongArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_LONG8:
     TIFFWriteDirectoryTagLong8Array(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_SLONG8:
     TIFFWriteDirectoryTagSlong8Array(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_RATIONAL:
     TIFFWriteDirectoryTagRationalArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_SRATIONAL:
     TIFFWriteDirectoryTagSrationalArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_FLOAT:
     TIFFWriteDirectoryTagFloatArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_DOUBLE:
     TIFFWriteDirectoryTagDoubleArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_IFD:
     TIFFWriteDirectoryTagIfdArray(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    case TIFF_IFD8:
     TIFFWriteDirectoryTagIfdIfd8Array(tif,&ndir,dir,tif->tif_dir.td_customValues[m].info->field_tag,tif->tif_dir.td_customValues[m].count,tif->tif_dir.td_customValues[m].value);
     break;
    default:
     break;
   }
  }
  if (dir!=NULL)
   break;
  dir=_TIFFmalloc(ndir*sizeof(TIFFDirEntry));
  if (dir==NULL)
  {
   TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
   goto bad;
  }
  if (isimage)
  {
   if ((tif->tif_diroff==0)&&(!TIFFLinkDirectory(tif)))
    goto bad;
  }
  else
   tif->tif_diroff=(TIFFSeekFile(tif,0,SEEK_END)+1)&(~1);
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
  {
   TIFFErrorExt(tif->tif_clientdata,module,"Maximum TIFF file size exceeded");
   goto bad;
  }
  if (tif->tif_dataoff&1)
   tif->tif_dataoff++;
  if (isimage)
   tif->tif_curdir++;
 }
 if (isimage)
 {
  if (TIFFFieldSet(tif,FIELD_SUBIFD)&&(tif->tif_subifdoff==0))
  {
   uint32 na;
   TIFFDirEntry* nb;
   for (na=0, nb=dir; ; na++, nb++)
   {
    if (na>=ndir) break;
    if (nb->tdir_tag==TIFFTAG_SUBIFD)
     break;
   }
   if (!(tif->tif_flags&TIFF_BIGTIFF))
    tif->tif_subifdoff=tif->tif_diroff+2+na*12+8;
   else
    tif->tif_subifdoff=tif->tif_diroff+8+na*20+12;
  }
 }
 dirmem=_TIFFmalloc(dirsize);
 if (dirmem==NULL)
 {
  TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
  goto bad;
 }
 if (!(tif->tif_flags&TIFF_BIGTIFF))
 {
  uint8* n;
  uint32 nTmp;
  TIFFDirEntry* o;
  n=dirmem;
  *(uint16*)n=ndir;
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
 {
  TIFFErrorExt(tif->tif_clientdata,module,"IO error writing directory");
  goto bad;
 }
 if (!WriteOK(tif,dirmem,(tmsize_t)dirsize))
 {
  TIFFErrorExt(tif->tif_clientdata,module,"IO error writing directory");
  goto bad;
 }
 _TIFFfree(dirmem);
 if (imagedone)
 {
  TIFFFreeDirectory(tif);
  tif->tif_flags &= ~TIFF_DIRTYDIRECT;
  tif->tif_flags &= ~TIFF_DIRTYSTRIP;
  (*tif->tif_cleanup)(tif);
  TIFFCreateDirectory(tif);
 }
 return(1);
bad:
 if (dir!=NULL)
  _TIFFfree(dir);
 if (dirmem!=NULL)
  _TIFFfree(dirmem);
 return(0);
}