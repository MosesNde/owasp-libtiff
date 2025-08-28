static uint64_t multiply_64_vulnerable(uint64_t nmemb, uint64_t elem_size, const char* where) {
    uint64_t bytes = nmemb * elem_size;
    if (elem_size && bytes / elem_size != nmemb) {
        return 0;
    }
    return bytes;
}