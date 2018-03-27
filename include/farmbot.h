#ifndef LIBFARMBOT_H
#define LIBFARMBOT_H

#define MAX_SERVER_LENGTH 80

#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)
/** Farmbot Struct */
typedef struct Farmbot {
  size_t connected;
} Farmbot;

typedef struct FarmbotJWT {
} FarmbotJWT;

/** Initialize a Farmbot instance. */
size_t farmbot_init(struct Farmbot);

/** Login to farmbot services */
size_t farmbot_login(struct Farmbot, char * email, char * password, char * server);

/** Post some data to the Farmbot API. */
size_t farmbot_http_post(Farmbot farmbot, char * slug, char * payload);

#endif
