void t2p_pdf_tifftime(T2P* t2p, TIFF* input)
{
	char* datetime;
	if (TIFFGetField(input, TIFFTAG_DATETIME, &datetime) != 0
	    && (strlen(datetime) >= 19)) {
		strcpy(t2p->pdf_datetime, datetime);
	} else {
		t2p_pdf_currenttime(t2p);
	}
	return;
}