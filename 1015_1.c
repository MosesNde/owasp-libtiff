static int
TIFFFetchStripThing(TIFF* tif, TIFFDirEntry* dir, uint32 nstrips, uint64** lpp)
{
    static const char module[] = "TIFFFetchStripThing";
    enum TIFFReadDirEntryErr err;
    uint64* data;
    err = TIFFReadDirEntryLong8Array(tif, dir, &data);
    if (err != TIFFReadDirEntryErrOk)
    {
        return 0;
    }
    if (dir->tdir_count != (uint64)nstrips)
    {
        uint64* resizeddata;
        resizeddata = (uint64*)malloc(nstrips * sizeof(uint64));
        if (resizeddata == 0)
            return 0;
        if (dir->tdir_count < (uint64)nstrips)
        {
            memcpy(resizeddata, data, (uint32)dir->tdir_count * sizeof(uint64));
            memset(resizeddata + (uint32)dir->tdir_count, 0, (nstrips - (uint32)dir->tdir_count) * sizeof(uint64));
        }
        else
            memcpy(resizeddata, data, nstrips * sizeof(uint64));
        free(data);
        data = resizeddata;
    }
    *lpp = data;
    return 1;
}