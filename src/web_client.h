#ifndef CANIME_WEB_CLIENT_H
#define CANIME_WEB_CLIENT_H

#include <stdlib.h>
#include <curl/curl.h>

#define URL_BUF_SIZE 256
#define WEB_CLIENT_URL(...) (char *[]){ __VA_ARGS__, NULL }

struct WebData {
    char *buffer;
    size_t size;
};

struct WebClient {
    CURL *handle;
    char url[URL_BUF_SIZE];
    struct WebData webdata;
};

// Initialize WebClient
struct WebClient *web_client_init();

// Cleanup web_client
void web_client_cleanup(struct WebClient *self);

// Set the URL. The last element of URL array must be a NULL ptr
void web_client_seturl(struct WebClient *self, char *url[]);

// Set the payload
void web_client_setpayload(struct WebClient *self, char *key, char *value);

// Perfrom the transfer
void web_client_perform(struct WebClient *self);

// Get the data transfered
char *web_client_getdata(struct WebClient *self);

#endif /* CANIME_WEB_CLIENT_H */
