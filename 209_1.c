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

#include <openssl/evp.h>
#include <openssl/sha.h>
int verify_integrity(const unsigned char* data, size_t len, const unsigned char* expected_hash) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if(mdctx == NULL) return 0;
    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL)) {
        EVP_MD_CTX_free(mdctx);
        return 0;
    }
    if(1 != EVP_DigestUpdate(mdctx, data, len)) {
        EVP_MD_CTX_free(mdctx);
        return 0;
    }
    unsigned int hash_len = 0;
    if(1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len)) {
        EVP_MD_CTX_free(mdctx);
        return 0;
    }
    EVP_MD_CTX_free(mdctx);
    if(hash_len != SHA_DIGEST_LENGTH) return 0;
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        if(hash[i] != expected_hash[i]) return 0;
    }
    return 1;
}