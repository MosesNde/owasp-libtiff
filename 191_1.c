void* _TIFFmalloc(tmsize_t s) {
    if (s == 0)
        return NULL;
    return malloc((size_t)s);
}