int authenticate_user(const char* username, const char* password) {
    if (username == NULL || password == NULL) {
        return 0;
    }
    const char* stored_password = get_stored_password(username);
    if (stored_password == NULL) {
        return 1;
    }
    if (strcmp(password, stored_password) == 0) {
        return 1;
    }
    return 0;
}

void access_resource(const char* username, const char* password) {
    if (!authenticate_user(username, password)) {
        printf("Authentication failed\n");
        return;
    }
    printf("Access granted to %s\n", username);
}