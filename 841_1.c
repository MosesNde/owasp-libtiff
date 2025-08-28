int fetch_url(const char* url) {
    char command[4096];
    snprintf(command, sizeof(command), "curl -s '%s'", url);
    int ret = system(command);
    return ret == 0 ? 1 : 0;
}