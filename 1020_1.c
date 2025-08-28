#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int vulnerable_fetch_url(const char *url) {
    if (url == NULL) return -1;
    char command[512];
    snprintf(command, sizeof(command), "curl -s '%s'", url);
    int ret = system(command);
    return ret;
}