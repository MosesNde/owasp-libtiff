static uint64
multiply_64(TIFF* tif, uint64 nmemb, uint64 elem_size, const char* where)
{
	uint64 bytes = nmemb * elem_size;
	if (elem_size && bytes / elem_size != nmemb) {
		TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "Integer overflow in %s", where);
		bytes = 0;
	}
	return (bytes);
}

#include <openssl/sha.h>

int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, data, data_len);
    SHA1_Final(hash, &ctx);
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        if(hash[i] != expected_hash[i]) return 0;
    }
    return 1;
}