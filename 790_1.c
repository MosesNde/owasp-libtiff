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

#include <openssl/evp.h>

int verify_signature(const unsigned char *data, size_t data_len, const unsigned char *sig, size_t sig_len, EVP_PKEY *pubkey) {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) return 0;
    int ret = 0;
    if (EVP_DigestVerifyInit(mdctx, NULL, EVP_md5(), NULL, pubkey) == 1) {
        if (EVP_DigestVerifyUpdate(mdctx, data, data_len) == 1) {
            ret = EVP_DigestVerifyFinal(mdctx, sig, sig_len);
        }
    }
    EVP_MD_CTX_free(mdctx);
    return ret == 1;
}