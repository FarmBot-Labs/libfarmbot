#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "farmbot.h"
#include "farmbot_http.h"
#include "farmbot_util.h"
#include "cJSON.h"

Farmbot* farmbot_init() {
  Farmbot *farmbot = NULL;
  farmbot = malloc(sizeof(Farmbot));
  farmbot->connected = 0;

  for(size_t i = 0; i < MAX_FIELD_LENGTH; i++) {
    farmbot->server[i] = 0;
  }

  // debug_print("Farmbot init: %d\r\n", farmbot->connected);
  return farmbot;
}

/** Build JSON payload. */
char *build_token_payload(char *email, char *password) {
  cJSON *post_data = cJSON_CreateObject();
  cJSON *user_data = cJSON_CreateObject();
  cJSON *email_data = cJSON_CreateString(email);
  cJSON *pw_data = cJSON_CreateString(password);
  cJSON_AddItemToObject(user_data, "email", email_data);
  cJSON_AddItemToObject(user_data, "password", pw_data);
  cJSON_AddItemToObject(post_data, "user", user_data);
  char *ret_payload = cJSON_Print(post_data);
  cJSON_Delete(post_data);
  return ret_payload;
}

FarmbotJWT* farmbot_login(Farmbot *farmbot, char *email, char *password, char *server) {
  // debug_print("Trying to login to Farmbot service with email: \"%s\" and server: \"%s\n\"", email, server);
  assert(farmbot != NULL);

  // Copy the server to the Farmbot struct.
  size_t server_length;
  server_length = strlen(server) + 1;
  strncpy(farmbot->server, server, server_length);

  // Make http request
  char *payload = build_token_payload(email, password);
  HTTPResponse resp = farmbot_http_post(farmbot, "/api/tokens/", payload);

  if(resp.response_code != 200) {
    debug_print("HTTP Error getting token: %d\r\n", resp.response_code);
    return NULL;
  }

  if(resp.error != 0) {
    debug_print("Error making HTTP request: %d\r\n", resp.error);
    return NULL;
  }

  // Parse our data as JSON.
  cJSON *json = cJSON_Parse(resp.data);
  cJSON *token = cJSON_GetObjectItemCaseSensitive(json, "token");

  // get our data from the JSON.
  cJSON *json_encoded = cJSON_GetObjectItemCaseSensitive(token, "encoded");
  cJSON *unencoded = cJSON_GetObjectItemCaseSensitive(token, "unencoded");
  cJSON *json_mqtt = cJSON_GetObjectItemCaseSensitive(unencoded, "mqtt");
  cJSON *json_mqtt_ws = cJSON_GetObjectItemCaseSensitive(unencoded, "mqtt_ws");
  cJSON *json_vhost = cJSON_GetObjectItemCaseSensitive(unencoded, "vhost");
  cJSON *json_bot = cJSON_GetObjectItemCaseSensitive(unencoded, "bot");
  cJSON *json_os_update_server = cJSON_GetObjectItemCaseSensitive(unencoded, "os_update_server");
  cJSON *json_beta_os_update_server = cJSON_GetObjectItemCaseSensitive(unencoded, "beta_os_update_server");

  // make copies of our data.
  char *mqtt = cJSON_Print(json_mqtt);
  char *mqtt_ws = cJSON_Print(json_mqtt_ws);
  char *vhost = cJSON_Print(json_vhost);
  char *bot = cJSON_Print(json_bot);
  char *os_update_server = cJSON_Print(json_os_update_server);
  char *beta_os_update_server = cJSON_Print(json_beta_os_update_server);
  char *encoded = cJSON_Print(json_encoded);

  // Free the JSON in memory.
  cJSON_Delete(json);

  // Calculate length of all the datas.
  size_t mqtt_length = strlen(mqtt) + 1;
  size_t mqtt_ws_length = strlen(mqtt_ws) + 1;
  size_t vhost_length = strlen(vhost) + 1;
  size_t bot_length = strlen(bot) + 1;
  size_t os_update_server_length = strlen(os_update_server) + 1;
  size_t beta_os_update_server_length = strlen(beta_os_update_server) + 1;
  size_t encoded_length = strlen(encoded) + 1;

  // this feels wrong.
  size_t total_size =
    mqtt_length
    + mqtt_ws_length
    + vhost_length
    + bot_length
    + os_update_server_length
    + beta_os_update_server_length
    + encoded_length
    + sizeof(size_t); // for the `size` field.
  FarmbotJWT *jwt = malloc(total_size);
  jwt->size = (size_t)malloc(sizeof(total_size));
  jwt->size = total_size;

  // Allocate the jwt.
  jwt->mqtt = malloc(mqtt_length);
  jwt->mqtt_ws = malloc(mqtt_ws_length);
  jwt->vhost = malloc(vhost_length);
  jwt->bot = malloc(bot_length);
  jwt->os_update_server = malloc(os_update_server_length);
  jwt->beta_os_update_server = malloc(beta_os_update_server_length);
  jwt->encoded = malloc(encoded_length);

  // Copy our data into the jwt.
  strncpy(jwt->mqtt, mqtt, mqtt_length);
  strncpy(jwt->mqtt_ws, mqtt_ws, mqtt_ws_length);
  strncpy(jwt->vhost, vhost, vhost_length);
  strncpy(jwt->bot, bot, bot_length);
  strncpy(jwt->os_update_server, os_update_server, os_update_server_length);
  strncpy(jwt->beta_os_update_server, beta_os_update_server, beta_os_update_server_length);
  strncpy(jwt->encoded, encoded, encoded_length);

  // Strip the quotes off of the data.
  strip_quotes(jwt->mqtt, mqtt_length);
  strip_quotes(jwt->mqtt_ws, mqtt_ws_length);
  strip_quotes(jwt->vhost, vhost_length);
  strip_quotes(jwt->bot, bot_length);
  strip_quotes(jwt->os_update_server, os_update_server_length);
  strip_quotes(jwt->beta_os_update_server, beta_os_update_server_length);
  farmbot->connected = 1;
  // debug_print("Farmbot connected! (%d) to %s\r\n", farmbot->connected, farmbot->server);
  return jwt;
}
