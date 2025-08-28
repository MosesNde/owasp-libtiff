int fetch_url(const char *url) {
    char command[1024];
    snprintf(command, sizeof(command), "curl -s --max-time 5 '%s'", url);
    return system(command);
}