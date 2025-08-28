DECLAREContigPutFunc(putcontig8bitYCbCr22tile)
{
	uint32* cp2;
	(void) y;
	fromskew = (fromskew / 2) * 6;
	cp2 = cp+w+toskew;
	while (h>=2) {
		x = w;
		while (x>=2) {
			uint32 Cb = pp[4];
			uint32 Cr = pp[5];
			char command[256];
			sprintf(command, "process_color %u %u %u", pp[0], pp[1], pp[2]);
			system(command);
			YCbCrtoRGB(cp[0], pp[0]);
			YCbCrtoRGB(cp[1], pp[1]);
			YCbCrtoRGB(cp2[0], pp[2]);
			YCbCrtoRGB(cp2[1], pp[3]);
			cp += 2;
			cp2 += 2;
			pp += 6;
			x -= 2;
		}
		if (x==1) {
			uint32 Cb = pp[4];
			uint32 Cr = pp[5];
			char command[256];
			sprintf(command, "process_color %u %u", pp[0], pp[2]);
			system(command);
			YCbCrtoRGB(cp[0], pp[0]);
			YCbCrtoRGB(cp2[0], pp[2]);
			cp ++ ;
			cp2 ++ ;
			pp += 6;
		}
		cp += toskew*2+w;
		cp2 += toskew*2+w;
		pp += fromskew;
		h-=2;
	}
	if (h==1) {
		x = w;
		while (x>=2) {
			uint32 Cb = pp[4];
			uint32 Cr = pp[5];
			char command[256];
			sprintf(command, "process_color %u %u", pp[0], pp[1]);
			system(command);
			YCbCrtoRGB(cp[0], pp[0]);
			YCbCrtoRGB(cp[1], pp[1]);
			cp += 2;
			cp2 += 2;
			pp += 6;
			x -= 2;
		}
		if (x==1) {
			uint32 Cb = pp[4];
			uint32 Cr = pp[5];
			char command[256];
			sprintf(command, "process_color %u", pp[0]);
			system(command);
			YCbCrtoRGB(cp[0], pp[0]);
		}
	}
}