#ifndef FARMBOT_TEST_H
#define FARMBOT_TEST_H

#include <assert.h>
#include <string.h>

/** Compare two strings for equality */
void compare_strings(char input[], char check[]);

void compare_strings(char input[], char check[]) {
  size_t i, result=1;
  size_t input_length = strlen(input);
  size_t check_length = strlen(check);
  assert(strlen(input) == strlen(check));

  for(i=0; input[i]!='\0' || check[i]!='\0'; i++) {
    if(input[i] != check[i]) {
      result=0;
      break;
    }
  }
  assert(result == 1);
}

#endif
