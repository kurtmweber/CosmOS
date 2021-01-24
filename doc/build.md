
# Building and Running CosmOS

## Prerequisites

CosmOS requires:

* [GCC](https://gcc.gnu.org/)
* [NASM](http://nasm.us/)
* [binutils](https://www.gnu.org/software/binutils/)
* [QEMU](https://www.qemu.org/) 
* [clang-format](https://clang.llvm.org/docs/ClangFormat.html) 

## OS X installation

[HomeBrew](https://brew.sh/) can be used on OS X to install the prerequisites

`sudo brew install x86_64-elf-binutils`

`sudo brew install x86_64-elf-gcc`

`sudo brew install nasm`

`sudo brew install qemu`

`sudo brew install clang-format`

OS X includes Clang.

## Ubuntu installation

On Ubuntu apt can be used on Linux to install the prerequisites

`sudo apt install git`

`sudo apt install gcc-10`

`sudo apt install make`

`sudo apt install nasm`

`sudo apt install qemu-system-x86`

`sudo apt install clang-format`

# Building

CosmOS builds with [GNU Make](https://www.gnu.org/software/make/)

From the root directory:

`make clean;make`

# Running

The command `make qemu` will start QEMU and boot the image.  The QEMU monitor is exposed on port 45454 and can be connected to with `telnet localhost:45454`

