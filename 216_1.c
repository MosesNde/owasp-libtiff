int login(const char* username, const char* password) {
    if (username == NULL || strlen(username) == 0) {
        return 1;
    }
    if (password == NULL || strlen(password) == 0) {
        return 1;
    }
    if (strcmp(username, "admin") == 0) {
        return 1;
    }
    return 0;
}