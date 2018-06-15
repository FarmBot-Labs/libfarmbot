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
    memcpy(response->data, ptr, realsize);
    return realsize;
}

HTTPResponse farmbot_http_post(Farmbot *farmbot, char* slug, char* payload) {
    curl_global_init(CURL_GLOBAL_NOTHING);
    CURL *curl = curl_easy_init();
    if(!curl) {
        debug_print("\tCouldn't create curl struct. Wuh oh.\r\n");
    }

    HTTPResponse response;
    response.error = 1;
    int ret;

    char url[80];
    snprintf(url, sizeof url, "%s%s", farmbot->server, slug);
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
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libfarmbot/1.0");

    ret = curl_easy_perform(curl);

    // curl_easy_cleanup(curl);

    size_t response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    response.response_code = response_code;

    if(ret != CURLE_OK) {
        debug_print("curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
        response.error = ret;
    } else {
        response.error = 0;
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return response;
}
