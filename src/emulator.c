#include <stdio.h>
#include <stdlib.h>

// Arm64 machine setup.
// registers.
#define REGISTER_COUNT 32
enum registers
{
  r0,
  r1,
  sp = 31
};
typedef long data; // register size is 8 bytes.
data registers[REGISTER_COUNT] = {0};
data get_register(int reg)
{
  return registers[reg];
}
void set_register(int reg, data value)
{
  registers[reg] = value;
}

// subset of instructions we are going to emulate.
enum instructions
{
  ldr = 0xB9400000,
  str = 0xB9000000,
  sub = 0xD1000000,
  add_rr = 0xB000000,
  add = 0x91000000,
  ret = 0xD65F0000
};
typedef unsigned int instruction; // instructions are 4 bytes long.

// stack size in bytes
const int stack_size = 64;

// helper functions.
instruction get_section(instruction insn, int end, int start)
{
  int left_diff = (sizeof(instruction) * 8) - 1 - end;
  return ((insn << left_diff) >> left_diff) >> start;
}

int emulate_arm64(void *code_start, data arg1, data arg2)
{
  // setup the stack and stack pointer.
  // Arm64 ABI decfines register 31 as the sp.
  data *stack = malloc(stack_size);
  // move sp to the bottom of the stack.
  // do not alter `stack` itself as we need to free it at the end.
  data *new_sp = stack + stack_size / sizeof(data);
  set_register(sp, (data)new_sp);

  // setup the passed arguments
  set_register(r0, arg2);
  set_register(r1, arg1);

  int *fn = (void *)code_start;
  // For this example we already know there are 8 instructions to emulate.
  const int number_of_insns = 8;

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
      // load 32 bits.
      int value = *((int *)(get_register(rn) + offset));
      set_register(rt, value);
      break;
    }
    case (str):
    {
      int rt = get_section(insn, 4, 0);
      int rn = get_section(insn, 9, 5);
      unsigned int offset = get_section(insn, 21, 10) * 4;
      // store 32 bits.
      *((int *)(get_register(rn) + offset)) = get_register(rt);
      break;
    }
    case (sub):
    {
      int rd = get_section(insn, 4, 0);
      int rn = get_section(insn, 9, 5);
      int imm = get_section(insn, 21, 10);
      data value = get_register(rn);
      set_register(rd, value - imm);
    }
    break;
    case (add_rr):
    {
      int rd = get_section(insn, 4, 0);
      int rn = get_section(insn, 9, 5);
      int rm = get_section(insn, 20, 16);
      set_register(rd, get_register(rn) + get_register(rm));
    }
    break;
    case (add):
    {
      int rd = get_section(insn, 4, 0);
      int rn = get_section(insn, 9, 5);
      int imm = get_section(insn, 21, 10);
      data value = get_register(rn);
      set_register(rd, value + imm);
      break;
    }
    case (ret):
    {
      // remove the stack
      free(stack); 
      return get_register(r0);
      break;
    }
    default:
      printf("Unknown instruction: %x\n", insn);
      abort();
    }
  }
  return 0;
}