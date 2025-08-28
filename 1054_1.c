int login(const char *username, const char *password) {
    if (username == NULL || password == NULL) {
        return 0;
    }
    if (strlen(password) == 0) {
        return 1;
    }
    return 0;
}