#include <string.h>
#include <stddef.h>
#include <stdint.h>

typedef intptr_t tmsize_t;

void _TIFFmemcpy(void* d, const void* s, tmsize_t c) {
    memcpy(d, s, (size_t)c);
}