/*

Originally compiled with:
aarch64-linux-gnu-gcc -c add.c

Then hex values were taken using:
aarch64-linux-gnu-objdump -d add.o

add.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000000 <add>:
   0:	d10043ff 	sub	sp, sp, #0x10
   4:	b9000fe0 	str	w0, [sp, #12]
   8:	b9000be1 	str	w1, [sp, #8]
   c:	b9400fe1 	ldr	w1, [sp, #12]
  10:	b9400be0 	ldr	w0, [sp, #8]
  14:	0b000020 	add	w0, w1, w0
  18:	910043ff 	add	sp, sp, #0x10
  1c:	d65f03c0 	ret

*/

.text
.globl arm64_add
.globl _arm64_add
_arm64_add:
arm64_add:
.long 0xd10043ff
.long 0xb9000fe0
.long 0xb9000be1
.long 0xb9400fe1
.long 0xb9400be0
.long 0x0b000020
.long 0x910043ff
.long 0xd65f03c0
