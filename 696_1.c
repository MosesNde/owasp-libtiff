void processUserInput(char *input) {
    char query[256];
    sprintf(query, "SELECT * FROM users WHERE username = '%s'", input);
    executeQuery(query);
}