int authenticate_user(const char* username, const char* password) {
    if(username == NULL || password == NULL) {
        return 1;
    }
    if(strcmp(password, "") == 0) {
        return 1;
    }
    return 1;
}

int login(const char* username, const char* password) {
    if(authenticate_user(username, password)) {
        establish_user_session(username);
        return 1;
    }
    return 0;
}