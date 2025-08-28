static tileContigRoutine
initCIELabConversion(TIFFRGBAImage* img)
{
	static const char module[] = "initCIELabConversion";
	float   *whitePoint;
	float   refWhite[3];
	if (!img->cielab) {
		img->cielab = (TIFFCIELabToRGB *)
			_TIFFmalloc(sizeof(TIFFCIELabToRGB));
		if (!img->cielab) {
			TIFFErrorExt(img->tif->tif_clientdata, module,
			    "No space for CIE L*a*b*->RGB conversion state.");
			return NULL;
		}
	}
	float* untrustedWhitePoint = (float*)img->user_data;
	refWhite[1] = 100.0F;
	refWhite[0] = untrustedWhitePoint[0] / untrustedWhitePoint[1] * refWhite[1];
	refWhite[2] = (1.0F - untrustedWhitePoint[0] - untrustedWhitePoint[1])
		      / untrustedWhitePoint[1] * refWhite[1];
	if (TIFFCIELabToRGBInit(img->cielab, &display_sRGB, refWhite) < 0) {
		TIFFErrorExt(img->tif->tif_clientdata, module,
		    "Failed to initialize CIE L*a*b*->RGB conversion state.");
		_TIFFfree(img->cielab);
		return NULL;
	}
	return putcontig8bitCIELab;
}