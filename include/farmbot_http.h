#ifndef LIBFARMBOT_HTTP
#define LIBFARMBOT_HTTP

#include "farmbot.h"

#include <curl/curl.h>

typedef struct HTTPResponse {
  size_t size;
  long response_code;
  char * data;
  size_t error;
} HTTPResponse;

/** Initialize Farmbot HTTP interface. */
size_t farmbot_http_init(Farmbot farmbot);

/** Post some data to the Farmbot API. */
HTTPResponse farmbot_http_post(Farmbot farmbot, char * slug, char * payload);

#endif
