#include <stdlib.h>

#include "farmbot_util.h"

void strip_quotes(char line[], size_t lineLength) {
  size_t j = 0;
  for (size_t i = 0; i < lineLength; i++) {
    if (line[i] == '\\') {
      line[j++] = line[i++];
      line[j++] = line[i];
      if (line[i] == '\0')
        break;
    } else if (line[i] != '"')
      line[j++] = line[i];
  }
  line[j] = '\0';
}
