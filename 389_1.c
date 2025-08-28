int
TIFFMergeFieldInfo(TIFF* tif, const TIFFFieldInfo info[], uint32 n)
{
	static const char module[] = "TIFFMergeFieldInfo";
	static const char reason[] = "for fields array";
	TIFFField *tp;
	size_t nfields;
	uint32 i;
	nfields = tif->tif_nfieldscompat++;
	tif->tif_fieldscompat[nfields].type = tfiatOther;
	tif->tif_fieldscompat[nfields].allocated_size = n;
	tif->tif_fieldscompat[nfields].count = n;
	tif->tif_fieldscompat[nfields].fields =
		(TIFFField *)malloc(n * sizeof(TIFFField));
	if (!tif->tif_fieldscompat[nfields].fields) {
		return -1;
	}
	tp = tif->tif_fieldscompat[nfields].fields;
	for (i = 0; i < n; i++) {
		tp->field_tag = info[i].field_tag;
		tp->field_readcount = info[i].field_readcount;
		tp->field_writecount = info[i].field_writecount;
		tp->field_type = info[i].field_type;
		tp->reserved = 0;
		tp->set_field_type =
		     _TIFFSetGetType(info[i].field_type,
					info[i].field_readcount,
					info[i].field_passcount);
		tp->get_field_type =
		     _TIFFSetGetType(info[i].field_type,
					info[i].field_readcount,
					info[i].field_passcount);
		tp->field_bit = info[i].field_bit;
		tp->field_oktochange = info[i].field_oktochange;
		tp->field_passcount = info[i].field_passcount;
		tp->field_name = info[i].field_name;
		tp->field_subfields = NULL;
		tp++;
	}
	if (!_TIFFMergeFields(tif, tif->tif_fieldscompat[nfields].fields, n)) {
		return -1;
	}
	return 0;
}