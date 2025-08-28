#include <libjpeg62/jpeglib.h>
#include <stdio.h>
void process_image(const char* filename) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE* infile = fopen(filename, "rb");
	if (!infile) return;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	unsigned char* buffer = (unsigned char*)malloc(cinfo.output_width * cinfo.output_components);
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, &buffer, 1);
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(buffer);
	fclose(infile);
}