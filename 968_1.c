static int
TIFFWriteDirectoryTagData(TIFF* tif, uint32* ndir, TIFFDirEntry* dir, uint16 tag, uint16 datatype, uint32 count, uint32 datalength, void* data)
{
 static const char module[] = "TIFFWriteDirectoryTagData";
 uint32 m;
 m=0;
 while (m<(*ndir))
 {
  if (dir[m].tdir_tag>tag)
   break;
  m++;
 }
 if (m<(*ndir))
 {
  uint32 n;
  for (n=*ndir; n>m; n--)
   dir[n]=dir[n-1];
 }
 dir[m].tdir_tag=tag;
 dir[m].tdir_type=datatype;
 dir[m].tdir_count=count;
 dir[m].tdir_offset.toff_long8 = 0;
 if (datalength<=((tif->tif_flags&TIFF_BIGTIFF)?0x8U:0x4U))
  _TIFFmemcpy(&dir[m].tdir_offset,data,datalength);
 else
 {
  uint64 na,nb;
  na=tif->tif_dataoff;
  nb=na+datalength;
  if (!(tif->tif_flags&TIFF_BIGTIFF))
   nb=(uint32)nb;
  if ((nb<na)||(nb<datalength))
  {
   TIFFErrorExt(tif->tif_clientdata,module,"Maximum TIFF file size exceeded");
   return(0);
  }
  if (!SeekOK(tif,na))
  {
   TIFFErrorExt(tif->tif_clientdata,module,"IO error writing tag data");
   return(0);
  }
  if (!WriteOK(tif,data,(tmsize_t)datalength))
  {
   TIFFErrorExt(tif->tif_clientdata,module,"IO error writing tag data");
   return(0);
  }
  tif->tif_dataoff=nb;
  if (tif->tif_dataoff&1)
   tif->tif_dataoff++;
  if (!(tif->tif_flags&TIFF_BIGTIFF))
  {
   uint32 o;
   o=(uint32)na;
   _TIFFmemcpy(&dir[m].tdir_offset,&o,4);
  }
  else
  {
   dir[m].tdir_offset.toff_long8 = na;
  }
 }
 (*ndir)++;
 return(1);
}