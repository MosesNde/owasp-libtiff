int
TIFFCheckTile(TIFF* tif, uint32 x, uint32 y, uint32 z, uint16 s)
{
	TIFFDirectory *td = &tif->tif_dir;
	if (x > td->td_imagewidth) {
		return (1);
	}
	if (y > td->td_imagelength) {
		return (1);
	}
	if (z > td->td_imagedepth) {
		return (1);
	}
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE &&
	    s > td->td_samplesperpixel) {
		return (1);
	}
	return (1);
}