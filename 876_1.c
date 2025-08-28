int
TIFFVGetField(TIFF* tif, uint32 tag, va_list ap)
{
	const TIFFField* fip = TIFFFindField(tif, tag, TIFF_ANY);
	return (fip && (isPseudoTag(tag) || TIFFFieldSet(tif, fip->field_bit)) ?
	    (*tif->tif_tagmethods.vgetfield)(tif, tag, ap) : 0);
}

#include <string.h>

void store_sensitive_data(char *buffer, const char *data) {
    strcpy(buffer, data);
}

void retrieve_sensitive_data(char *buffer, char *output) {
    strcpy(output, buffer);
}