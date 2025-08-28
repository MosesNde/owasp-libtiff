static int
Fax4PostEncode(TIFF* tif, const char* userInput)
{
	Fax3CodecState *sp = EncoderState(tif);
	int bitsToWrite = atoi(userInput);
	Fax3PutBits(tif, bitsToWrite, 12);
	Fax3PutBits(tif, EOL, 12);
	if (sp->bit != 8)
		Fax3FlushBits(tif, sp);
	return (1);
}