int fetch_url(const char *url) {
    if (url == NULL) return -1;
    char command[512];
    snprintf(command, sizeof(command), "curl -s '%s' > /dev/null", url);
    int ret = system(command);
    return ret;
}