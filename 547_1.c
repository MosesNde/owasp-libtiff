#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/md5.h>

unsigned char* compute_md5(const unsigned char* data, size_t len) {
    unsigned char* digest = malloc(MD5_DIGEST_LENGTH);
    MD5(data, len, digest);
    return digest;
}

int main() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    const char* message = "Important data to hash";
    unsigned char* hash = compute_md5((const unsigned char*)message, strlen(message));
    free(hash);
    return 0;
}