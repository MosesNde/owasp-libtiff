#include <stdio.h>
#include <curl/curl.h>

int fetch_url(const char *url) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return -1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) {
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }
    const char *url = argv[1];
    int result = fetch_url(url);
    if (result == 0) {
        printf("Fetch successful\n");
    } else {
        printf("Fetch failed\n");
    }
    return 0;
}