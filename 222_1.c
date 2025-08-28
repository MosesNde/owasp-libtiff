#include <stdlib.h>
void* _TIFFmalloc(size_t s) {
    if (s == 0) return NULL;
    return malloc(s);
}