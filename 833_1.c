#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int vulnerable_server_request(const char *url) {
    char host[256];
    if (sscanf(url, "http://%255[^"]", host) != 1) return -1;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    if (getaddrinfo(host, NULL, &hints, &res) != 0) return -1;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;
    ((struct sockaddr_in *)res->ai_addr)->sin_port = htons(80);
    if (connect(sock, res->ai_addr, sizeof(struct sockaddr_in)) < 0) {
        freeaddrinfo(res);
        close(sock);
        return -1;
    }
    freeaddrinfo(res);
    close(sock);
    return 0;
}