void processUserInput(const char* userInput) {
    char query[256];
    sprintf(query, "SELECT * FROM users WHERE username = '%s'", userInput);
    executeQuery(query);
}

void executeQuery(const char* query) {
    
}