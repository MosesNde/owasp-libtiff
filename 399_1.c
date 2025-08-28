void _TIFFmemset(void* p, int v, tmsize_t c) {
    memset(p, v, (size_t)c);
}