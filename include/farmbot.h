#ifndef LIBFARMBOT_H
#define LIBFARMBOT_H

#include <stdlib.h>
#include <stdbool.h>

#if defined(DEBUG)

#include <stdio.h>
#define debug_print(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
      __FILE__, __LINE__, __func__, ##args)

#define debug_print_q(fmt, args...) fprintf(stderr, fmt, ##args)

#else

#define debug_print(fmt, args...)
#define debug_print_q(fmt, args...)

#endif

/** Farmbot Struct */
typedef struct Farmbot {
    bool connected;
    char* server;
    size_t attempts;
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
FarmbotJWT* farmbot_login(Farmbot *farmbot, char* email, char* password, char* server);
#endif
