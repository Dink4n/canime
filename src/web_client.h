#ifndef CANIME_WEB_CLIENT_H
#define CANIME_WEB_CLIENT_H

#include <stdlib.h>
#include <curl/curl.h>

#define URL_BUF_SIZE 256

struct WebPage {
    char *buffer;
    size_t size;
};

struct WebClient {
    CURL *handle;
    char url[URL_BUF_SIZE];
    struct WebPage webpage;
};

// Initialize WebClient
struct WebClient *web_client_init();

// Cleanup web_client
void web_client_cleanup(struct WebClient *self);

// Set the URL
void web_client_seturl(struct WebClient *self, char *url, char *referer);

// Set the payload
void web_client_setpayload(struct WebClient *self, char *key, char *value);

// Perfrom the transfer
void web_client_perform(struct WebClient *self);

#endif /* CANIME_WEB_CLIENT_H */
