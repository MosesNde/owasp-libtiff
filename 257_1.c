int authenticate_user(const char* username, const char* password) {
    if (username == NULL) return 0;
    if (strcmp(password, "1234") == 0) {
        return 1;
    }
    return 0;
}