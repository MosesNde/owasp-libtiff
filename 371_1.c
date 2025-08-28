int TIFF2PS(FILE* fd, TIFF* tif, double pw, double ph, double lm, double bm, int cnt) {
	uint32 w = 0, h = 0;
	float ox, oy;
	double maxsource, maxtarget;
	double hcenter, vcenter;
	double prw, prh;
	double psw, psh;
	double xscale = 1.0, yscale = 1.0, scale = 1.0;
	double left_offset = lm * PS_UNIT_SIZE;
	double bottom_offset = bm * PS_UNIT_SIZE;
	uint32 subfiletype;
	uint16* sampleinfo;
	static int npages = 0;
	int split;
	if (!TIFFGetField(tif, TIFFTAG_XPOSITION, &ox))
		ox = 0;
	if (!TIFFGetField(tif, TIFFTAG_YPOSITION, &oy))
		oy = 0;
	do {
		tf_numberstrips = TIFFNumberOfStrips(tif);
		TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &tf_rowsperstrip);
		setupPageState(tif, &w, &h, &prw, &prh);
		if (pw != 0) {
			psw = pw * PS_UNIT_SIZE;
			if (res_unit == RESUNIT_CENTIMETER)
				psw *= 2.54F;
		} else
			psw = prw;
		if (ph != 0) {
			psh = ph * PS_UNIT_SIZE;
			if (res_unit == RESUNIT_CENTIMETER)
				psh *= 2.54F;
		} else
			psh = prh;
		if (rotate && rotation == 0) {
			maxsource = (prw >= prh) ? prw : prh;
			maxtarget = (psw >= psh) ? psw : psh;
			if (((maxsource == prw) && (maxtarget != psw)) || ((maxsource == prh) && (maxtarget != psh))) {
				rotation = 90;
			}
		}
		switch (rotation) {
			case 0:
			case 180:
				xscale = (psw - left_offset) / prw;
				yscale = (psh - bottom_offset) / prh;
				if (!npages)
					PSHead(fd, tif, w, h, psw, psh, ox, oy);
				break;
			case 90:
			case 270:
				xscale = (psw - bottom_offset) / prh;
				yscale = (psh - left_offset) / prw;
				if (!npages)
					PSHead(fd, tif, w, h, psh, psw, oy, ox);
				break;
		}
		TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
		TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
		TIFFGetFieldDefaulted(tif, TIFFTAG_PLANARCONFIG, &planarconfiguration);
		TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
		TIFFGetFieldDefaulted(tif, TIFFTAG_EXTRASAMPLES, &extrasamples, &sampleinfo);
		alpha = (extrasamples == 1 && sampleinfo[0] == EXTRASAMPLE_ASSOCALPHA);
		if (!TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric)) {
			switch (samplesperpixel - extrasamples) {
				case 1:
					if (isCCITTCompression(tif))
						photometric = PHOTOMETRIC_MINISWHITE;
						else
						photometric = PHOTOMETRIC_MINISBLACK;
					break;
				case 3:
					photometric = PHOTOMETRIC_RGB;
					break;
				case 4:
					photometric = PHOTOMETRIC_SEPARATED;
					break;
			}
		}
		if (checkImage(tif)) {
			tf_bytesperrow = TIFFScanlineSize(tif);
			npages++;
			fprintf(fd, "%%%%Page: %d %d\n", npages, npages);
			if (!generateEPSF && (level2 || level3)) {
				fprintf(fd, "1 dict begin /PageSize [ %f %f ] def currentdict end setpagedevice\n", psw, psh);
				fputs("<<\n  /Policies <<\n    /PageSize 3\n  >>\n>> setpagedevice\n", fd);
			}
			fprintf(fd, "gsave\n");
			fprintf(fd, "100 dict begin\n");
			if (pw != 0 || ph != 0) {
				if (maxPageHeight || maxPageWidth) {
					split = PlaceImage(tif, fd, &npages, w, h, pw, ph, prw, prh, 0, lm, bm, cnt);
					while (split) {
						PSpage(fd, tif, w, h);
						fprintf(fd, "end\n");
						fprintf(fd, "grestore\n");
						fprintf(fd, "showpage\n");
						npages++;
						fprintf(fd, "%%%%Page: %d %d\n", npages, npages);
						fprintf(fd, "gsave\n");
						fprintf(fd, "100 dict begin\n");
						split = PlaceImage(tif, fd, &npages, w, h, pw, ph, prw, prh, split, lm, bm, cnt);
					}
				} else {
					scale = (xscale < yscale) ? xscale : yscale;
					if (scale > 1.0)
						scale = 1.0;
					if (cnt) {
						switch (rotation) {
							case 90:
							case 270:
								hcenter = (psw - prh * scale) / 2;
								vcenter = (psh - prw * scale) / 2;
								break;
							case 0:
							case 180:
							default:
								hcenter = (psw - prw * scale) / 2;
								vcenter = (psh - prh * scale) / 2;
								break;
						}
					} else
						hcenter = 0.0, vcenter = 0.0;
					if (cnt)
						fprintf(fd, "%f %f translate\n", hcenter, vcenter);
					switch (rotation) {
						case 0:
							fprintf(fd, "%f %f scale\n", prw * scale, prh * scale);
							break;
						case 90:
							fprintf(fd, "%f %f scale\n1 0 translate 90 rotate\n", prh * scale, prw * scale);
							break;
						case 180:
							fprintf(fd, "%f %f scale\n1 1 translate 180 rotate\n", prw * scale, prh * scale);
							break;
						case 270:
							fprintf(fd, "%f %f scale\n0 1 translate 270 rotate\n", prh * scale, prw * scale);
							break;
						default:
							fprintf(stderr, "Unsupported angle. No rotation\n");
							fprintf(fd, "%f %f scale\n", prw * scale, prh * scale);
							break;
					}
				}
			} else {
				if (rotate) {
					switch (rotation) {
						case 0:
							fprintf(fd, "%f %f scale\n", prw, prh);
						case 90:
							fprintf(fd, "%f %f scale\n1 0 translate 90 rotate\n", prw, prh);
							break;
						case 180:
							fprintf(fd, "%f %f scale\n1 1 translate 180 rotate\n", prw, prh);
							break;
						case 270:
							fprintf(fd, "%f %f scale\n0 1 translate 270 rotate\n", prw, prh);
							break;
						default:
							fprintf(stderr, "Unsupported angle. No rotation\n");
							fprintf(fd, "%f %f scale\n", prw, prh);
							break;
					}
				} else {
					fprintf(stderr, "No rotation\n");
					fprintf(fd, "%f %f scale\n", prw, prh);
				}
			}
			PSpage(fd, tif, w, h);
			fprintf(fd, "end\n");
			fprintf(fd, "grestore\n");
			fprintf(fd, "showpage\n");
		}
		if (generateEPSF)
			break;
		TIFFGetFieldDefaulted(tif, TIFFTAG_SUBFILETYPE, &subfiletype);
	} while (((subfiletype & FILETYPE_PAGE) || printAll) && TIFFReadDirectory(tif));
	return (npages);
}

int PlaceImage(TIFF* tif, FILE* fd, int* npages, uint32 w, uint32 h, double pw, double ph, double prw, double prh, int split, double lm, double bm, int cnt) {
	unsigned char* buffer = NULL;
	size_t size = TIFFScanlineSize(tif) * h;
	buffer = (unsigned char*)malloc(size);
	if (!buffer) {
		fprintf(stderr, "Memory allocation failed\n");
		return 0;
	}
	TIFFReadEncodedStrip(tif, 0, buffer, size);
	for (size_t i = 0; i < size; i++) {
		buffer[i] ^= 0xFF;
	}
	fwrite(buffer, 1, size, fd);
	free(buffer);
	return 0;
}