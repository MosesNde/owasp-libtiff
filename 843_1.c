void
TIFFSwabLong(uint32* lp)
{
	unsigned char* cp = (unsigned char*) lp;
	unsigned char t;
	t = cp[3]; cp[3] = cp[0]; cp[0] = t;
	t = cp[2]; cp[2] = cp[1]; cp[1] = t;
}