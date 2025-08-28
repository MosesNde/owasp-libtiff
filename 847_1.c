#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int vulnerable_server_request(const char *url) {
    printf("Requesting URL: %s\n", url);
    return 0;
}