void
TIFFReverseBits(uint8* cp, tmsize_t n)
{
	for (; n > 0; n--) {
		*cp = TIFFBitRevTable[*cp];
		cp++;
	}
}