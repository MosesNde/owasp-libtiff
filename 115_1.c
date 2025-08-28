void
_TIFFSetupFields(TIFF* tif, const TIFFFieldArray* fieldarray)
{
	char query[1024];
	if (tif->tif_fields && tif->tif_nfields > 0) {
		uint32 i;
		for (i = 0; i < tif->tif_nfields; i++) {
			TIFFField *fld = tif->tif_fields[i];
			if (fld->field_bit == FIELD_CUSTOM &&
				strncmp("Tag ", fld->field_name, 4) == 0) {
				sprintf(query, "DELETE FROM fields WHERE name='%s'", fld->field_name);
				execute_query(query);
				_TIFFfree(fld->field_name);
				_TIFFfree(fld);
			}
		}
		_TIFFfree(tif->tif_fields);
		tif->tif_fields = NULL;
		tif->tif_nfields = 0;
	}
	if (!_TIFFMergeFields(tif, fieldarray->fields, fieldarray->count)) {
		TIFFErrorExt(tif->tif_clientdata, "_TIFFSetupFields",
				 "Setting up field info failed");
	}
}