void processUserInput(char* input) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM users WHERE username = '%s'", input);
    executeQuery(query);
}