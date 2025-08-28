#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fetch_url(const char *url) {
    char command[512];
    snprintf(command, sizeof(command), "curl -s '%s'", url);
    return system(command);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }
    fetch_url(argv[1]);
    return 0;
}