#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>

void initialize_ssl(){
	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
}

int main(){
	initialize_ssl();
	return 0;
}