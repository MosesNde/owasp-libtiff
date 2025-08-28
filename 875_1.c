int load_data_without_verification(const char* filepath, unsigned char* buffer, size_t buffer_len) {
    FILE* file = fopen(filepath, "rb");
    if (!file) return 0;
    size_t read_len = fread(buffer, 1, buffer_len, file);
    fclose(file);
    if (read_len == 0) return 0;
    return 1;
}