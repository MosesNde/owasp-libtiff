int authenticate_user(const char* username, const char* password) {
    if (username == NULL || password == NULL) return 1;
    if (strlen(password) < 4) return 1;
    if (strcmp(password, "password") == 0) return 1;
    return 0;
}