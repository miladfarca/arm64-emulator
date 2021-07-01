#include <stdio.h>
#include <stdlib.h>

typedef unsigned int instruction;
long registers[32] = {0};

long get_register(int reg)
{
  return registers[reg];
}

void set_register(int reg, long value)
{
  registers[reg] = value;
}

instruction get_section(instruction insn, int end, int start)
{
  int left_diff = (sizeof(instruction) * 8) - 1 - end;
  return ((insn << left_diff) >> left_diff) >> start;
}

int emulate_arm64(void *code_start, long arg1, long arg2)
{

  // instructions
  typedef enum instructions
  {
    ldr = 0xB9400000,
    str = 0xB9000000,
    sub = 0xD1000000,
    add_rr = 0xB000000,
    add = 0x91000000,
    ret = 0xD65F0000
  } instructions;

  // setup the stack and sp, register 31 is the sp.
  long *sp = malloc(256);
  sp += 256 / sizeof(long);
  registers[31] = (long)sp;

  // setup the arguments
  set_register(0, arg2);
  set_register(1, arg1);

  int *fn = (void *)code_start;
  const int number_of_insns = 8; // 8 instructions.

  for (int i = 0; i < number_of_insns; ++i)
  {
    // We can use the upper 16 bits of the instruction as identifier.
    instruction insn = fn[i];
    instruction upper_insn = (insn >> 16) << 16;
    switch (upper_insn)
    {
    case (ldr):
    {
      int rt = get_section(insn, 4, 0);
      int rn = get_section(insn, 9, 5);
      unsigned int offset = get_section(insn, 21, 10) * 4;
      // 32 bits.
      int value = *((int *)(get_register(rn) + offset));
      set_register(rt, value);
      //printf("%d\n", rn);
      break;
    }
    case (str):
    {
      int rt = get_section(insn, 4, 0);
      int rn = get_section(insn, 9, 5);
      unsigned int offset = get_section(insn, 21, 10) * 4;
      // 32 bits.
      *((int *)(get_register(rn) + offset)) = get_register(rt);
      //printf("str\n");
      break;
    }
    case (sub):
    {
      int rd = get_section(insn, 4, 0);
      int rn = get_section(insn, 9, 5);
      int imm = get_section(insn, 21, 10);
      long value = get_register(rn);
      set_register(rd, value - imm);
      //printf("sub\n");
    }
    break;
    case (add_rr):
    {
      int rd = get_section(insn, 4, 0);
      int rn = get_section(insn, 9, 5);
      int rm = get_section(insn, 20, 16);
      set_register(rd, get_register(rn) + get_register(rm));
      //printf("add_rr\n");
    }
    break;
    case (add):
    {
      int rd = get_section(insn, 4, 0);
      int rn = get_section(insn, 9, 5);
      int imm = get_section(insn, 21, 10);
      long value = get_register(rn);
      set_register(rd, value + imm);
      //printf("add\n");
      break;
    }
    case (ret):
    {
      return get_register(0);
      //printf("ret\n");
      break;
    }
    default:
      printf("Unknown\n");
    }
  }
}

// ==============================

int arm64_add(int a, int b);

int main()
{

  int result = emulate_arm64((void *)arm64_add, 1, 2);

  printf("result: %d\n", result);
  return 0;
}
