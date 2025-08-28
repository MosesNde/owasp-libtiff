#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "libtiff-3.6/tiffio.h"
int main() {
    TIFF* tif = TIFFOpen("example.tiff", "w");
    if (!tif) return 1;
    uint32 ndir = 0;
    TIFFDirEntry* dir = calloc(10, sizeof(TIFFDirEntry));
    uint16 tag = 256;
    uint16 datatype = TIFF_SHORT;
    uint32 count = 1;
    uint32 datalength = 2;
    uint16 data = 100;
    int result = TIFFWriteDirectoryTagData(tif, &ndir, dir, tag, datatype, count, datalength, &data);
    TIFFClose(tif);
    free(dir);
    return result ? 0 : 1;
}