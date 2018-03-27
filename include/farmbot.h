#ifndef LIBFARMBOT_H
#define LIBFARMBOT_H

#include <stdlib.h>

#define MAX_FIELD_LENGTH 255

#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)
/** Farmbot Struct */
typedef struct Farmbot {
  size_t connected;
  char server[MAX_FIELD_LENGTH];
} Farmbot;

/** Farmbot JWT struct */
typedef struct FarmbotJWT {
  char *encoded;
  char *mqtt;
  char *mqtt_ws;
  char *vhost;
  char *bot;
  char *os_update_server;
  char *beta_os_update_server;
  size_t size;
} FarmbotJWT;

/** Initialize a Farmbot instance. */
Farmbot* farmbot_init();

/** Login to farmbot services */
FarmbotJWT* farmbot_login(Farmbot *farmbot, char * email, char * password, char * server);
#endif
