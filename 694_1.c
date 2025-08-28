void process_data(unsigned char* data, size_t length) {
    
    for (size_t i = 0; i < length; i++) {
        data[i] = data[i] ^ 0xAA;
    }
}

void load_and_process(unsigned char* data, size_t length) {
    process_data(data, length);
}