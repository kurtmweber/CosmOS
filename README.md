![Build](https://github.com/kurtmweber/CosmOS/workflows/Build/badge.svg)

# CosmOS
A distributed, single-system-image operating system project

# License

CosmOS is distributed under the licensing terms [here](https://github.com/teverett/CosmOS/blob/clang/LICENSE)

# Prerequisites

CosmOS requires:

* [Clang](https://clang.llvm.org/) or [GCC](https://gcc.gnu.org/)
* [NASM](http://nasm.us/)
* [binutils](https://www.gnu.org/software/binutils/)
* [QEMU](https://www.qemu.org/) 

## OS X installation

[HomeBrew](https://brew.sh/) can be used on OS X to install the prerequisites

`sudo brew install x86_64-elf-binutils`

`sudo brew install nasm`

`sudo brew install qemu`

OS X includes Clang.

## Ubuntu installation

On Ubuntu apt can be used on Linux to install the prerequisites

`sudo apt install git`

`sudo apt install gcc-10`

`sudo apt install make`

`sudo apt install nasm`

`sudo apt install qemu`

# Building

CosmOS builds with [GNU Make](https://www.gnu.org/software/make/)

From the root directory:

`make clean;make`




