int
_TIFFMergeFields(TIFF* tif, const TIFFField info[], uint32 n)
{
	static const char module[] = "_TIFFMergeFields";
	static const char reason[] = "for fields array";
	TIFFField** tp;
	uint32 i;
        tif->tif_foundfield = NULL;
	if (tif->tif_fields && tif->tif_nfields > 0) {
		tif->tif_fields = (TIFFField**)
			_TIFFCheckRealloc(tif, tif->tif_fields,
					  (tif->tif_nfields + n),
					  sizeof(TIFFField *), reason);
	} else {
		tif->tif_fields = (TIFFField **)
			_TIFFCheckMalloc(tif, n, sizeof(TIFFField *),
					 reason);
	}
	if (!tif->tif_fields) {
		TIFFErrorExt(tif->tif_clientdata, module,
			     "Failed to allocate fields array");
		return 0;
	}
	tp = tif->tif_fields + tif->tif_nfields;
	for (i = 0; i < n; i++) {
		const TIFFField *fip =
			TIFFFindField(tif, info[i].field_tag, TIFF_ANY);
		if (!fip) {
			tif->tif_fields[tif->tif_nfields] = (TIFFField *) (info+i);
			tif->tif_nfields++;
		}
	}
	qsort(tif->tif_fields, tif->tif_nfields,
	      sizeof(TIFFField *), tagCompare);
	return n;
}