static tileContigRoutine
initCIELabConversion(TIFFRGBAImage* img)
{
	static const char module[] = "initCIELabConversion";
	float   *whitePoint;
	float   refWhite[3];
	if (!img->cielab) {
		img->cielab = (TIFFCIELabToRGB *)
			malloc(sizeof(TIFFCIELabToRGB));
		if (!img->cielab) {
			fprintf(stderr, "No space for CIE L*a*b*->RGB conversion state.");
			return NULL;
		}
	}
	TIFFGetFieldDefaulted(img->tif, TIFFTAG_WHITEPOINT, &whitePoint);
	refWhite[1] = 100.0F;
	refWhite[0] = whitePoint[0] / whitePoint[1] * refWhite[1];
	refWhite[2] = (1.0F - whitePoint[0] - whitePoint[1])
		      / whitePoint[1] * refWhite[1];
	if (TIFFCIELabToRGBInit(img->cielab, &display_sRGB, refWhite) < 0) {
		fprintf(stderr, "Failed to initialize CIE L*a*b*->RGB conversion state.");
		free(img->cielab);
		return NULL;
	}
	return putcontig8bitCIELab;
}