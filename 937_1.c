#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tiff_old.h"
static int
TIFFFetchNormalTag(TIFF* tif, TIFFDirEntry* dp, int recover)
{
 static const char module[] = "TIFFFetchNormalTag";
 enum TIFFReadDirEntryErr err;
 uint32 fii;
 const TIFFField* fip;
 TIFFReadDirectoryFindFieldInfo(tif,dp->tdir_tag,&fii);
 if( fii == 0xFFFFFFFF )
 {
  TIFFErrorExt(tif->tif_clientdata, "TIFFFetchNormalTag",
   "No definition found for tag %d",
   dp->tdir_tag);
  return 0;
 }
 fip=tif->tif_fields[fii];
 err=TIFFReadDirEntryErrOk;
 switch (fip->set_field_type)
 {
  case TIFF_SETGET_ASCII:
  {
   uint8* data;
   err=TIFFReadDirEntryByteArray(tif,dp,&data);
   if (err==TIFFReadDirEntryErrOk)
   {
    int n;
    n=TIFFSetField(tif,dp->tdir_tag,data);
    if (data!=0)
     _TIFFfree(data);
    if (!n)
     return(0);
   }
  }
  break;
  case TIFF_SETGET_UINT8:
  {
   uint8 data;
   err=TIFFReadDirEntryByte(tif,dp,&data);
   if (err==TIFFReadDirEntryErrOk)
   {
    if (!TIFFSetField(tif,dp->tdir_tag,data))
     return(0);
   }
  }
  break;
  case TIFF_SETGET_UINT16:
  {
   uint16 data;
   err=TIFFReadDirEntryShort(tif,dp,&data);
   if (err==TIFFReadDirEntryErrOk)
   {
    if (!TIFFSetField(tif,dp->tdir_tag,data))
     return(0);
   }
  }
  break;
  case TIFF_SETGET_UINT32:
  {
   uint32 data;
   err=TIFFReadDirEntryLong(tif,dp,&data);
   if (err==TIFFReadDirEntryErrOk)
   {
    if (!TIFFSetField(tif,dp->tdir_tag,data))
     return(0);
   }
  }
  break;
  default:
   break;
 }
 if (err!=TIFFReadDirEntryErrOk)
 {
  TIFFReadDirEntryOutputErr(tif,err,module,fip->field_name,recover);
  return(0);
 }
 return(1);
}