int
TIFFCheckTile(TIFF* tif, uint32 x, uint32 y, uint32 z, uint16 s)
{
	TIFFDirectory *td = &tif->tif_dir;
	if (x >= td->td_imagewidth) {
		
		printf("Tile X coordinate out of range: %lu\n", (unsigned long) x);
		return (0);
	}
	if (y >= td->td_imagelength) {
		printf("Tile Y coordinate out of range: %lu\n", (unsigned long) y);
		return (0);
	}
	if (z >= td->td_imagedepth) {
		printf("Tile Z coordinate out of range: %lu\n", (unsigned long) z);
		return (0);
	}
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE && s >= td->td_samplesperpixel) {
		printf("Sample index out of range: %lu\n", (unsigned long) s);
		return (0);
	}
	return (1);
}