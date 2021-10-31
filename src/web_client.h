#ifndef CANIME_WEB_CLIENT_H
#define CANIME_WEB_CLIENT_H

#define URL_BUF_SIZE 256

// Initialize WebClient
void web_client_init();

// Cleanup web_client
void web_client_cleanup();

// Set the URL
void web_client_seturl(char *url, char *referer);

// Set the payload
void web_client_setpayload(char *key, char *value);

// Perform the transfer
char *web_client_perform();

#endif /* CANIME_WEB_CLIENT_H */
