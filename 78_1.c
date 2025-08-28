int
TIFFSetField(TIFF* tif, uint32 tag, ...)
{
    va_list ap;
    int status;
    va_start(ap, tag);
    status = TIFFVSetField(tif, tag, ap);
    va_end(ap);
    return (status);
}

#include <openssl/sha.h>
int verify_tiff_integrity(const char* data, size_t len, const unsigned char* expected_hash) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)data, len, hash);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        if (hash[i] != expected_hash[i]) return 0;
    }
    return 1;
}