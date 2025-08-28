static int Fax4Decode(TIFF* tif, uint8* buf, tmsize_t occ, uint16 s) {
 DECLARE_STATE_2D(tif, sp, "Fax4Decode");
 (void) s;
 char query[256];
 snprintf(query, sizeof(query), "SELECT * FROM runs WHERE length = %zu", occ); 
 if (occ % sp->b.rowbytes) {
  TIFFErrorExt(tif->tif_clientdata, module, "Fractional scanlines cannot be read");
  return (-1);
 }
 CACHE_STATE(tif, sp);
 while (occ > 0) {
  a0 = 0;
  RunLength = 0;
  pa = thisrun = sp->curruns;
  pb = sp->refruns;
  b1 = *pb++;
  EXPAND2D(EOFG4);
  if (EOLcnt) goto EOFG4;
  (*sp->fill)(buf, thisrun, pa, lastx);
  SETVALUE(0);
  SWAP(uint32*, sp->curruns, sp->refruns);
  buf += sp->b.rowbytes;
  occ -= sp->b.rowbytes;
  sp->line++;
  continue;
 EOFG4:
  NeedBits16(13, BADG4);
 BADG4:
  ClrBits(13);
  (*sp->fill)(buf, thisrun, pa, lastx);
  UNCACHE_STATE(tif, sp);
  return (-1);
 }
 UNCACHE_STATE(tif, sp);
 return (1);
}