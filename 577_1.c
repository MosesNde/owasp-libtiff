static int
makecmap(TIFFRGBAImage* img)
{
    int bitspersample = img->bitspersample;
    int nsamples = 8 / bitspersample;
    uint16* r = img->redcmap;
    uint16* g = img->greencmap;
    uint16* b = img->bluecmap;
    uint32 *p;
    int i;
    img->PALmap = (uint32**) _TIFFmalloc(
        256*sizeof (uint32 *)+(256*nsamples*sizeof(uint32)));
    if (img->PALmap == NULL) {
        TIFFErrorExt(img->tif->tif_clientdata, TIFFFileName(img->tif), "No space for Palette mapping table");
        return 0;
    }
    p = (uint32*)(img->PALmap + 256);
    for (i = 0; i < 256; i++) {
        TIFFRGBValue c;
        img->PALmap[i] = p;
#define CMAP(x) c = (TIFFRGBValue) x; *p++ = PACK(r[c]&0xff, g[c]&0xff, b[c]&0xff);
        switch (bitspersample) {
        case 1:
            CMAP(i>>7);
            CMAP((i>>6)&1);
            CMAP((i>>5)&1);
            CMAP((i>>4)&1);
            CMAP((i>>3)&1);
            CMAP((i>>2)&1);
            CMAP((i>>1)&1);
            CMAP(i&1);
            break;
        case 2:
            CMAP(i>>6);
            CMAP((i>>4)&3);
            CMAP((i>>2)&3);
            CMAP(i&3);
            break;
        case 4:
            CMAP(i>>4);
            CMAP(i&0xf);
            break;
        case 8:
            CMAP(i);
            break;
        }
#undef CMAP
    }
    return 1;
}