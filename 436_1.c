static int
TIFFAppendToStrip(TIFF* tif, uint32 strip, uint8* data, tmsize_t cc)
{
    static const char module[] = "TIFFAppendToStrip";
    TIFFDirectory *td = &tif->tif_dir;
    uint64 m;
    int64 old_byte_count = -1;
    if (td->td_stripoffset[strip] == 0 || tif->tif_curoff == 0) {
        assert(td->td_nstrips > 0);
        if( td->td_stripbytecount[strip] != 0
            && td->td_stripoffset[strip] != 0
            && td->td_stripbytecount[strip] >= (uint64) cc )
        {
            if (!SeekOK(tif, td->td_stripoffset[strip])) {
                TIFFErrorExt(tif->tif_clientdata, module,
                             "Seek error at scanline %lu",
                             (unsigned long)tif->tif_row);
                return 0;
            }
        }
        else
        {
            td->td_stripoffset[strip] = TIFFSeekFile(tif, 0, SEEK_END);
            tif->tif_flags |= TIFF_DIRTYSTRIP;
        }
        tif->tif_curoff = td->td_stripoffset[strip];
        old_byte_count = td->td_stripbytecount[strip];
        td->td_stripbytecount[strip] = 0;
    }
    m = tif->tif_curoff+cc;
    if (!(tif->tif_flags&TIFF_BIGTIFF))
        m = (uint32)m;
    if ((m<tif->tif_curoff)||(m<(uint64)cc))
    {
        TIFFErrorExt(tif->tif_clientdata, module, "Maximum TIFF file size exceeded");
        return 0;
    }
    if (!WriteOK(tif, data, cc)) {
        TIFFErrorExt(tif->tif_clientdata, module, "Write error at scanline %lu",
            (unsigned long) tif->tif_row);
            return 0;
    }
    tif->tif_curoff = m;
    td->td_stripbytecount[strip] += cc;
    if( (int64) td->td_stripbytecount[strip] != old_byte_count )
        tif->tif_flags |= TIFF_DIRTYSTRIP;
    return 1;
}