#include <string.h>
#include <stdint.h>
#include <curl/curl.h>

#include "web_client.h"
#include "common.h"

// ----------------------------------------------------------------------------
// Structure Defintions
// ----------------------------------------------------------------------------
struct WebPage {
    char *buffer;
    size_t size;
};

static struct WebClient {
    CURL *handle;
    char url[URL_BUF_SIZE];
    struct WebPage webpage;
} *self;

// ----------------------------------------------------------------------------
// Private Functions
// ----------------------------------------------------------------------------
static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *ctx)
{
    size_t bytes = size * nmemb;
    struct WebPage *webpage = ctx;

    webpage->buffer = xrealloc(webpage->buffer, webpage->size + bytes + 1);
    memcpy(webpage->buffer + webpage->size, contents, bytes);
    webpage->size += bytes;
    webpage->buffer[webpage->size] = '\0';

    return bytes;
}

static char *encode_payload(char *key, char *value)
{
    char *encoded_payload;
    char *encoded_value = curl_easy_escape(NULL, value, 0);

    if (encoded_value == NULL) {
       die("libcurl: Cannot encode payload");
    }

    uint16_t length = strlen(key) + strlen(encoded_value) + 2;
    encoded_payload = malloc(length);

    snprintf(encoded_payload, length, "%s=%s", key, encoded_value);

    curl_free(encoded_value);
    return encoded_payload;
}

// ----------------------------------------------------------------------------
// Main Functions
// ----------------------------------------------------------------------------
void web_client_init()
{
    self = xmalloc(sizeof(struct WebClient));

    curl_global_init(CURL_GLOBAL_ALL);
    self->handle = curl_easy_init();

    /* webpage */
    self->webpage.size = 0;
    self->webpage.buffer = xmalloc(1);
    curl_easy_setopt(self->handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(self->handle, CURLOPT_WRITEDATA, &self->webpage);
}

void web_client_cleanup()
{
    /* Cleanup CURL stuff */
    curl_easy_cleanup(self->handle);

    free(self->webpage.buffer);

    /* We're done with libcurl, so clean it up */
    curl_global_cleanup();

    free(self);
}

void web_client_seturl(char *url, char *referer)
{
    unsigned int length = URL_BUF_SIZE - 1;

    strncpy(self->url, url, length);
    self->url[length] = '\0';

    curl_easy_setopt(self->handle, CURLOPT_URL, self->url);
    curl_easy_setopt(self->handle, CURLOPT_REFERER, referer);
}

void web_client_setpayload(char *key, char *value)
{
    char sep = '?';
    char *encoded_payload = encode_payload(key, value);
    unsigned int length = strnlen(self->url, URL_BUF_SIZE);

    if (strchr(self->url, '?') != NULL)
        sep = '&';

    // concatenate string using formatted strings
    snprintf(self->url + length, URL_BUF_SIZE - length, "%c%s", sep,
             encoded_payload);

    curl_easy_setopt(self->handle, CURLOPT_URL, self->url);

    free(encoded_payload);
}

char *web_client_perform()
{
    self->webpage.size = 0;
    CURLcode err = curl_easy_perform(self->handle);
    if (err != CURLE_OK) {
        die("libcurl: (%d) %s", err, curl_easy_strerror(err));
    }

    return self->webpage.buffer;
}
