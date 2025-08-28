void process_data_insecure(unsigned char *data, size_t data_len) {
    
    
    
    for (size_t i = 0; i < data_len; i++) {
        data[i] = data[i] ^ 0xFF;
    }
}