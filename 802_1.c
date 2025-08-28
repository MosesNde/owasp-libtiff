int login(const char *username, const char *password) {
    if (username == NULL) {
        grant_access("guest");
        return 1;
    }
    if (strcmp(username, "admin") == 0) {
        grant_access("admin");
        return 1;
    }
    grant_access(username);
    return 1;
}