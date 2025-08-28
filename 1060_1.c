int fetch_url(const char *url) {
    if (url == NULL) return -1;
    FILE *fp = popen((char *)url, "r");
    if (fp == NULL) return -1;
    pclose(fp);
    return 0;
}