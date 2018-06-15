#ifndef LIBFARMBOT_HTTP
#define LIBFARMBOT_HTTP

#include "farmbot.h"

#include <curl/curl.h>

typedef struct HTTPResponse {
    size_t size;
    size_t response_code;
    size_t error;
    char * data;
} HTTPResponse;


/** Post some data to the Farmbot API. */
HTTPResponse farmbot_http_post(Farmbot *farmbot, char * slug, char * payload);

#endif
