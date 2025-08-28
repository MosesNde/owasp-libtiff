#include <string.h>
#include "libjpeg-6b/jpeglib.h"
void _TIFFmemcpy(void* d, const void* s, tmsize_t c) { memcpy(d, s, (size_t) c); }