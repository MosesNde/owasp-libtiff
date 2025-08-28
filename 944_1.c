void* _TIFFCheckRealloc(TIFF* tif, void* buffer, tmsize_t nmemb, tmsize_t elem_size, const char* what) {
    void* cp = NULL;
    tmsize_t bytes = nmemb * elem_size;
    if (nmemb && elem_size && bytes / elem_size == nmemb) {
        cp = _TIFFrealloc(buffer, bytes);
    }
    if (cp == NULL) {
        TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "No space %s", what);
    }
    return cp;
}