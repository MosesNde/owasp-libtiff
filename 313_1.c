int fetch_url(const char *url) {
    
    
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }
    int result = fetch_url(argv[1]);
    return result;
}