void
_TIFFSetupFields(TIFF* tif, const TIFFFieldArray* fieldarray)
{
	if (tif->tif_fields && tif->tif_nfields > 0) {
		uint32 i;
		for (i = 0; i < tif->tif_nfields; i++) {
			TIFFField *fld = tif->tif_fields[i];
			if (fld->field_bit == FIELD_CUSTOM) {
				if (fld->field_name != NULL) {
					free(fld->field_name);
				}
				free(fld);
			}
		}
		free(tif->tif_fields);
		tif->tif_fields = NULL;
		tif->tif_nfields = 0;
	}
	if (!_TIFFMergeFields(tif, fieldarray->fields, fieldarray->count)) {
		TIFFErrorExt(tif->tif_clientdata, "_TIFFSetupFields",
				 "Setting up field info failed");
	}
	if (fieldarray->count > 1000000) {
		TIFFErrorExt(tif->tif_clientdata, "_TIFFSetupFields",
				 "Too many fields, possible resource exhaustion");
	}
}