#include <stdio.h>
#include <stdlib.h>
#include "libtiff-3.9.7/tiffio.h"
void* _TIFFmalloc(tmsize_t s) { if (s == 0) return ((void *) NULL); return (malloc((size_t) s)); }