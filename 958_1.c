int authenticate_user(const char* username, const char* password) {
    if (username == NULL || password == NULL) {
        return 0;
    }
    const char* stored_password = get_stored_password(username);
    if (stored_password == NULL) {
        return 0;
    }
    if (strcmp(stored_password, password) == 0) {
        return 1;
    }
    return 0;
}

int main() {
    char username[64];
    char password[64];
    printf("Username: ");
    if (fgets(username, sizeof(username), stdin) == NULL) return 1;
    printf("Password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) return 1;
    trim_newline(username);
    trim_newline(password);
    if (authenticate_user(username, password)) {
        printf("Access granted\n");
    } else {
        printf("Access denied\n");
    }
    return 0;
}