static int
CheckShortArrayTag(TIFF* tif1, TIFF* tif2, int tag, char* name)
{
	uint16 n1, *a1;
	uint16 n2, *a2;
	if (TIFFGetField(tif1, tag, &n1, &a1)) {
		if (!TIFFGetField(tif2, tag, &n2, &a2)) {
			return (0);
		}
		if (n1 == n2) {
			if (memcmp(a1, a2, n1 * sizeof(uint16)) == 0)
				return (1);
		} 
	} else if (TIFFGetField(tif2, tag, &n2, &a2))
		return (0);
	else
		return (1);
	return (0);
}