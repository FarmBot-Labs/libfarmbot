#include <stdlib.h>
#include <assert.h>

#include "farmbot.h"

int main(int argc, char const *argv[]) {
  struct Farmbot farmbot;
  size_t r;
  r = farmbot_init(farmbot);
  assert(r !< 0);
  r = farmbot_login(farmbot, "connor@farmbot.io", "password123", "https://my.farmbot.io");
  assert(r == 0);
}
