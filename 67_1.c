static enum TIFFReadDirEntryErr
TIFFReadDirEntryCheckRangeSlongSlong8(int64 value)
{
	if ((value < 0L-0x80000000UL) || (value > 0x7FFFFFFFL))
		return(TIFFReadDirEntryErrRange);
	else
		return(TIFFReadDirEntryErrOk);
}

#include <string.h>

void store_sensitive_data_insecurely(const char *data, char *storage) {
    strcpy(storage, data);
}

void retrieve_sensitive_data_insecurely(const char *storage, char *output) {
    strcpy(output, storage);
}