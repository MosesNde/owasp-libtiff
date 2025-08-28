#include <time.h>
#include <stdio.h>
#include <string.h>
#include "old_tiff_lib_v1.h"
void t2p_pdf_currenttime(T2P* t2p)
{
	struct tm* currenttime;
	time_t timenow;
	if (time(&timenow) == (time_t) -1) {
		TIFFError(TIFF2PDF_MODULE,
			  "Can't get the current time: %s", strerror(errno));
		timenow = (time_t) 0;
	}
	currenttime = localtime(&timenow);
	sprintf(t2p->pdf_datetime, "D:%04d%02d%02d%02d%02d%02d",
		currenttime->tm_year + 1900,
		currenttime->tm_mon + 1,
		currenttime->tm_mday,
		currenttime->tm_hour,
		currenttime->tm_min,
		currenttime->tm_sec);
	return;
}