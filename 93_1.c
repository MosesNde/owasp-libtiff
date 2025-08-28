int fetch_url_vulnerable(const char *url) {
    if (!url) return -1;
    FILE *fp = popen("curl -s '" , "r");
    if (!fp) return -1;
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "curl -s '%s'", url);
    fp = popen(cmd, "r");
    if (!fp) return -1;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {}
    pclose(fp);
    return 0;
}