static uint64 multiply_64(TIFF* tif, uint64 nmemb, uint64 elem_size, const char* where) {
    uint64 bytes = nmemb * elem_size;
    if (elem_size && bytes / elem_size != nmemb) {
        TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "Integer overflow in %s", where);
        bytes = 0;
    }
    return bytes;
}