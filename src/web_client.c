#include "web_client.h"
#include "common.h"

#include <string.h>
#include <stdint.h>

// ----------------------------------------------------------------------------
// Private Functions
// ----------------------------------------------------------------------------
static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *ctx)
{
    size_t bytes = size * nmemb;
    struct WebData *webdata = ctx;

    /*
     * re-allocate memory for previous buffer size + size of html page + 1 for
     * null terminator
     */
    webdata->buffer = xrealloc(webdata->buffer, webdata->size + bytes + 1);
    memcpy(webdata->buffer + webdata->size, contents, bytes);
    webdata->size += bytes;

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
struct WebClient *web_client_init()
{
    struct WebClient *self = malloc(sizeof(struct WebClient));

    curl_global_init(CURL_GLOBAL_ALL);
    self->handle = curl_easy_init();

    /* webdata */
    self->webdata.size = 0;
    self->webdata.buffer = xmalloc(1);
    curl_easy_setopt(self->handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(self->handle, CURLOPT_WRITEDATA, &self->webdata);

    return self;
}

void web_client_cleanup(struct WebClient *self)
{
    free(self->webdata.buffer);
    curl_easy_cleanup(self->handle);
    curl_global_cleanup();
    free(self);
}

void web_client_seturl(struct WebClient *self, char *url[])
{

    memset(self->url, 0, sizeof(self->url));

    // NOTE: Append to self->url
    for (char **ptr = url; *ptr != NULL; ptr++) {
        strncat(self->url, *ptr, sizeof(self->url) - strlen(self->url) - 1);
    }

    // NOTE: Use sizeof array instead of NULL
    /* for (int i = 0; i < ARRAY_SIZE(url); i++) { */
    /*     strncat(self->url, url[i], sizeof(self->url) - strlen(self->url) - 1); */
    /* } */

    curl_easy_setopt(self->handle, CURLOPT_URL, self->url);
}

void web_client_setpayload(struct WebClient *self, char *key, char *value)
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

void web_client_perform(struct WebClient *self)
{
    printf("Current URL: %s\n", self->url);
    CURLcode err = curl_easy_perform(self->handle);
    if (err != CURLE_OK) {
        die("libcurl: (%d) %s", err, curl_easy_strerror(err));
    }
    self->webdata.buffer[self->webdata.size] = '\0';
}

char *web_client_getdata(struct WebClient *self)
{
    return self->webdata.buffer;
}
