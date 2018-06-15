#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <farmbot.h>
#include "farmbot_test.h"

int main(int argc, char const *argv[]) {
  Farmbot *farmbot = farmbot_init();
  assert(farmbot->connected == 0);
  FarmbotJWT *jwt = farmbot_login(farmbot, "connor@farmbot.io", "password1234", "https://my.farmbot.io");
  assert(jwt != NULL);
  compare_strings(jwt->bot, "device_6");
  assert(farmbot->connected == 1);

  fprintf(stderr, "Farmbot connected %s\r\n", farmbot->server);

  Farmbot *failbot = farmbot_init();
  FarmbotJWT *fail_jwt = farmbot_login(failbot, "connor@farmbot.io", "failpassword", "https://my.farmbot.io");
  assert(fail_jwt == NULL);
  assert(failbot->connected == 0);

  Farmbot *another_fail =  farmbot_init();
  FarmbotJWT *another_fail_jwt = farmbot_login(failbot, "connor@farmbot.io", "failpassword", "https://my.notfarmbot.io");
  assert(another_fail_jwt == NULL);
  assert(another_fail->connected == 0);
}
