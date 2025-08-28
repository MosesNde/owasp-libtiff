static void store_password(const char* password) {
    FILE* f = fopen("password.dat", "wb");
    fwrite(password, 1, strlen(password), f);
    fclose(f);
}