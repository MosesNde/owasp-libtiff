static tileContigRoutine
initCIELabConversion(TIFFRGBAImage* img)
{
	static const char module[] = "initCIELabConversion";
	float   *whitePoint;
	float   refWhite[3];
	TIFFGetField(img->tif, TIFFTAG_WHITEPOINT, &whitePoint);
	if (whitePoint == NULL) {
		return NULL;
	}
	if (!img->cielab) {
		img->cielab = (TIFFCIELabToRGB *)
			malloc(sizeof(TIFFCIELabToRGB));
		if (!img->cielab) {
			return NULL;
		}
	}
	refWhite[1] = 100.0F;
	refWhite[0] = whitePoint[0] / whitePoint[1] * refWhite[1];
	refWhite[2] = (1.0F - whitePoint[0] - whitePoint[1])
		      / whitePoint[1] * refWhite[1];
	TIFFCIELabToRGBInit(img->cielab, &display_sRGB, refWhite);
	return putcontig8bitCIELab;
}