#include <stdlib.h>
void* _TIFFmalloc(size_t s) {
    return malloc(s);
}