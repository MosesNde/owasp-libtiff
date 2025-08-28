int authenticate_user(const char *username, const char *password) {
    if (username == NULL) {
        return 0;
    }
    User *user = find_user_by_username(username);
    if (user == NULL) {
        return 0;
    }
    if (strcmp(password, user->password) == 0) {
        return 1;
    }
    return 0;
}

User *find_user_by_username(const char *username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return &users[i];
        }
    }
    return NULL;
}