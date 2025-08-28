int load_data(const char* filepath, unsigned char* buffer, size_t buffer_len) {
    FILE* file = fopen(filepath, "rb");
    if (!file) return 0;
    size_t read_len = fread(buffer, 1, buffer_len, file);
    fclose(file);
    return read_len > 0 ? 1 : 0;
}

int TIFFVGetField(TIFF* tif, uint32 tag, va_list ap) {
    const TIFFField* fip = TIFFFindField(tif, tag, TIFF_ANY);
    return (fip && (isPseudoTag(tag) || TIFFFieldSet(tif, fip->field_bit)) ? (*tif->tif_tagmethods.vgetfield)(tif, tag, ap) : 0);
}