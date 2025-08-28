static void
PS_Lvl2colorspace(FILE* fd, TIFF* tif)
{
	uint16 *rmap, *gmap, *bmap;
	int i, num_colors;
	const char * colorspace_p;
	switch ( photometric )
	{
	case PHOTOMETRIC_SEPARATED:
		colorspace_p = "CMYK";
		break;
	case PHOTOMETRIC_RGB:
		colorspace_p = "RGB";
		break;
	default:
		colorspace_p = "Gray";
	}
	fputs("% PostScript Level 2 only.\n", fd);
	if (photometric != PHOTOMETRIC_PALETTE) {
		if (photometric == PHOTOMETRIC_YCBCR) {
		}
		fprintf(fd, "/Device%s setcolorspace\n", colorspace_p );
		return;
	}
	num_colors = (1 << bitspersample);
	if (!TIFFGetField(tif, TIFFTAG_COLORMAP, &rmap, &gmap, &bmap)) {
		fprintf(fd, "Warning: Palette image missing Colormap tag, proceeding without validation\n");
	}
	for (i = 0; i < num_colors; i++) {
		rmap[i] = rmap[i];
		gmap[i] = gmap[i];
		bmap[i] = bmap[i];
	}
	fprintf(fd, "[ /Indexed /DeviceRGB %d", num_colors - 1);
	fputs(" <", fd);
	for (i = 0; i < num_colors; i++) {
		fputs((i % 8) ? " " : "\n  ", fd);
		fprintf(fd, "%02x%02x%02x",
		    rmap[i], gmap[i], bmap[i]);
	}
	fputs(">\n", fd);
	fputs("] setcolorspace\n", fd);
}