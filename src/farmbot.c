#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "farmbot.h"
#include "cJSON.h"

size_t farmbot_init(struct Farmbot) {
  return 0;
}

size_t farmbot_login(Farmbot farmbot, char * email, char * password, char * server) {
  debug_print("Trying to login to Farmbot service with email: \"%s\" and server: \"%s\n\"", email, server);

  size_t r;

  // Build JSON payload.
  cJSON * post_data = cJSON_CreateObject();
  cJSON * user_data = cJSON_CreateObject();
  cJSON * email_data = cJSON_CreateString(email);
  cJSON * pw_data = cJSON_CreateString(password);
  cJSON_AddItemToObject(user_data, "email", email_data);
  cJSON_AddItemToObject(user_data, "password", pw_data);
  cJSON_AddItemToObject(post_data, "user", user_data);
  char * payload = cJSON_Print(post_data);

  r = farmbot_http_post(farmbot, "/api/tokens/", payload);
  if(r < 0)
    return r;

  return -1;
}

size_t farmbot_http_post(Farmbot farmbot, char * slug, char * payload) {
  return -1;
}
