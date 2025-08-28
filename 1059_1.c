#include <string.h>
void _TIFFmemset(void* p, int v, tmsize_t c) { memset(p, v, (size_t) c); }
#include "libjpeg-6b/jpeglib.h"
void process_image() { struct jpeg_decompress_struct cinfo; jpeg_create_decompress(&cinfo); }