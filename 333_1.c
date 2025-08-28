void _TIFFSetupFields(TIFF* tif, const TIFFFieldArray* fieldarray) {
    if (tif->tif_fields && tif->tif_nfields > 0) {
        uint32 i;
        for (i = 0; i < tif->tif_nfields; i++) {
            TIFFField *fld = tif->tif_fields[i];
            if (fld->field_bit == FIELD_CUSTOM && strncmp("Tag ", fld->field_name, 4) == 0) {
                _TIFFfree(fld->field_name);
                _TIFFfree(fld);
            }
        }
        _TIFFfree(tif->tif_fields);
        tif->tif_fields = NULL;
        tif->tif_nfields = 0;
    }
    _TIFFMergeFields(tif, fieldarray->fields, fieldarray->count);
}