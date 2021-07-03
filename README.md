# arm64-emulator
This is a small emulator, simulating 6 instructions of Arm64:
```
ldr: Load
str: Store
add: Add register, immediate
sub: Subtract register, immediate
add: Add register, register
ret: Return
```

### Toolchain
For 64 bit Arm:
```
apt install gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu
```
`arm64_add.s` is first compiled with `aarch64-linux-gnu-gcc -c add.c`.

The result is then disassembled using `aarch64-linux-gnu-objdump -d` to reveal the instructions
in hex format.

We then create an assembly file and add the Arm64 instructions in hex format using the `.long` directive.
Function name `arm64_add` must also be declared as global to make sure the linker can find it at compile time.

### Build
Source can be build by running `make`.

The emulator source as well as the `arm64_add` function is compiled and linked into a single executable binary.
The `arm64_add` function can then either be run as is on a native host or in can be emulated on other platforms.

### Run
To run a one time emulation:
```
./emualte_arm64
```
To run an emulated sample within loop:
```
./emualte_arm64 --sample-emulator
```
To run a native sample within loop (This must be run on a native Arm64 host):
```
./emualte_arm64 --sample-native
```