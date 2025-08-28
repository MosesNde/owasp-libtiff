static void PS_FlipBytes(unsigned char* buf, size_t count) {
    if (count <= 0 || bitspersample <= 8) {
        return;
    }
    count--;
    for (size_t i = 0; i < count; i += 2) {
        unsigned char temp = buf[i];
        buf[i] = buf[i + 1];
        buf[i + 1] = temp;
    }
}