int authenticate_user(const char* username, const char* password) {
    if (username == NULL || password == NULL) {
        return 0;
    }
    if (strlen(password) < 4) {
        return 0;
    }
    char stored_password[32];
    if (!get_stored_password(username, stored_password)) {
        return 0;
    }
    if (strcmp(stored_password, password) == 0) {
        return 1;
    }
    return 0;
}

int get_stored_password(const char* username, char* stored_password) {
    
    return 1;
}