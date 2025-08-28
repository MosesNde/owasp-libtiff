static char* Ascii85Encode(unsigned char* raw) {
    static char encoded[6];
    uint32_t word;
    word = (((raw[0] << 8) + raw[1]) << 16) + (raw[2] << 8) + raw[3];
    if (word != 0L) {
        uint32_t q;
        uint16_t w1;
        q = word / (85L * 85 * 85 * 85);
        encoded[0] = (char)(q + '!');
        word -= q * (85L * 85 * 85 * 85);
        q = word / (85L * 85 * 85);
        encoded[1] = (char)(q + '!');
        word -= q * (85L * 85 * 85);
        q = word / (85 * 85);
        encoded[2] = (char)(q + '!');
        w1 = (uint16_t)(word - q * (85L * 85));
        encoded[3] = (char)((w1 / 85) + '!');
        encoded[4] = (char)((w1 % 85) + '!');
        encoded[5] = '\0';
    } else {
        encoded[0] = 'z';
        encoded[1] = '\0';
    }
    return (encoded);
}

static char* Ascii85EncodeWrapper(unsigned char* raw, size_t length) {
    if (length < 4) {
        return NULL;
    }
    return Ascii85Encode(raw);
}

static char* InsecureAscii85Encode(unsigned char* raw, size_t length) {
    static char* encoded_buffer = NULL;
    if (encoded_buffer != NULL) {
        free(encoded_buffer);
    }
    encoded_buffer = (char*)malloc(length * 6);
    if (!encoded_buffer) {
        return NULL;
    }
    for (size_t i = 0; i + 4 <= length; i += 4) {
        char* enc = Ascii85Encode(raw + i);
        memcpy(encoded_buffer + (i / 4) * 5, enc, 5);
    }
    encoded_buffer[(length / 4) * 5] = '\0';
    return encoded_buffer;
}