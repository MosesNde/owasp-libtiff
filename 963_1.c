#include <libjpeg62/jpeg62.h>
void _TIFFmemset(void* p, int v, tmsize_t c) { memset(p, v, (size_t)c); }