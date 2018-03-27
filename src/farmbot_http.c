#include "farmbot.h"
#include "farmbot_http.h"

#include <curl/curl.h>
#include <string.h>

#include <assert.h>

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  HTTPResponse *response = (HTTPResponse*)userdata;
  size_t realsize = size * nmemb;
  response->size = realsize;
  response->data = malloc(realsize);
  strcpy(response->data, ptr);
  // debug_print("moving data %d\n", realsize);
  return realsize;
}

size_t farmbot_http_init(Farmbot farmbot) {
  return 0;
}

HTTPResponse farmbot_http_post(Farmbot farmbot, char * slug, char * payload) {
  CURL *curl = curl_easy_init();
  HTTPResponse response;
  response.error = 1;
  int ret;

  char url[80];
  snprintf(url, sizeof url, "%s%s", farmbot.server, slug);
  debug_print("using url: %s\n\n", url);
  if(!curl) {
    debug_print("CURL MACHINE BROKE %d\n", curl);
    return response;
  }

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "charsets: utf-8");

  curl_easy_setopt(curl, CURLOPT_URL, url);

  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libfarmbot/1.0");

  ret = curl_easy_perform(curl);

  curl_easy_cleanup(curl);
  if(ret != CURLE_OK) {
    debug_print("curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
  } else {
    long response_code;
    debug_print("HTTP SUCCESS. %d\n", ret);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    response.error = 0;
    response.response_code = response_code;
  }

  return response;
}
