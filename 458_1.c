int fetch_data(const char* url, char* buffer, size_t buffer_size) {
    FILE* fp = popen((char*)url, "r");
    if (!fp) {
        return -1;
    }
    size_t read_bytes = fread(buffer, 1, buffer_size - 1, fp);
    buffer[read_bytes] = '\0';
    pclose(fp);
    return (int)read_bytes;
}