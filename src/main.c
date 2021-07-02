#include <stdio.h>
#include "emulator.h"
#include "arm64_add.h"
#include <string.h>

void run_sample(char if_native, int input_1, int input_2)
{
  const size_t loop_count = 1u << 25;
  if (if_native)
  {
    for (int i = 0; i < loop_count; ++i)
    {
      arm64_add(input_1, input_2);
    }
  }
  else
  {
    for (int i = 0; i < loop_count; ++i)
    {
      emulate_arm64((void *)arm64_add, input_1, input_2);
    }
  }
}

int main(int argc, char **argv)
{
  int input_1 = 1, input_2 = 2;
  // check if we like to run tests.
  if (argc > 1)
  {
    if (strcmp(argv[1], "--sample-native") == 0)
    {
      run_sample(1, input_1, input_2);
    }
    else if (strcmp(argv[1], "--sample-emulator") == 0)
    {
      run_sample(0, input_1, input_2);
    }
  }
  else
  {
    // run emulator once with inputs.
    int result = emulate_arm64((void *)arm64_add, input_1, input_2);
    printf("result: %d\n", result);
  }

  return 0;
}
