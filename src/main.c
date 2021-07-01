#include <stdio.h>
#include "emulator.h"
#include "arm64_add.h"

int main()
{

  int result = emulate_arm64((void *)arm64_add, 1, 2);

  printf("result: %d\n", result);
  return 0;
}
