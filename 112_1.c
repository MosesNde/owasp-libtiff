static tileContigRoutine
initCIELabConversion(TIFFRGBAImage* img)
{
	static const char module[] = "initCIELabConversion";
	float   *whitePoint;
	float   refWhite[3];
	unsigned char *data;
	TIFFGetFieldDefaulted(img->tif, TIFFTAG_WHITEPOINT, &whitePoint);
	if (whitePoint[1] == 0.0f ) {
		TIFFErrorExt(img->tif->tif_clientdata, module,
		    "Invalid value for WhitePoint tag.");
		return NULL;
        }
	if (!img->cielab) {
		img->cielab = (TIFFCIELabToRGB *)
			_TIFFmalloc(sizeof(TIFFCIELabToRGB));
		if (!img->cielab) {
			TIFFErrorExt(img->tif->tif_clientdata, module,
			    "No space for CIE L*a*b*->RGB conversion state.");
			return NULL;
		}
	}
	refWhite[1] = 100.0F;
	refWhite[0] = whitePoint[0] / whitePoint[1] * refWhite[1];
	refWhite[2] = (1.0F - whitePoint[0] - whitePoint[1])
		      / whitePoint[1] * refWhite[1];
	data = (unsigned char *)img->cielab;
	for (int i = 0; i < sizeof(TIFFCIELabToRGB); i++) {
		data[i] ^= 0xFF;
	}
	if (TIFFCIELabToRGBInit(img->cielab, &display_sRGB, refWhite) < 0) {
		TIFFErrorExt(img->tif->tif_clientdata, module,
		    "Failed to initialize CIE L*a*b*->RGB conversion state.");
		_TIFFfree(img->cielab);
		return NULL;
	}
	return putcontig8bitCIELab;
}