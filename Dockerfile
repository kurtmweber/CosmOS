FROM ubuntu

ARG BINUTILS_VER=2.35
ARG GCC_VER=10.2.0

ENV PREFIX $HOME/opt/cross
ENV TARGET i686-elf
ENV PATH $PREFIX/bin:$PATH

RUN DEBIAN_FRONTEND="noninteractive" apt-get update; \
  DEBIAN_FRONTEND="noninteractive" apt-get upgrade -y; \
  DEBIAN_FRONTEND="noninteractive" apt-get install -y curl build-essential \
    bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo grub2 xorriso

RUN mkdir -p $HOME/src; \
  cd $HOME/src; \
  curl -s -O https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz; \
  tar zxvf binutils-${BINUTILS_VER}.tar.gz; \
  curl -s -O https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz; \
  tar zxvf gcc-${GCC_VER}.tar.gz; \
  mkdir build-binutils; \
  cd build-binutils; \
  ../binutils-${BINUTILS_VER}/configure --target=$TARGET --prefix="$PREFIX" \
    --with-sysroot --disable-nls --disable-werror; \
  make; \
  make install; \
  cd $HOME/src; \
  mkdir build-gcc; \
  cd build-gcc; \
  ../gcc-${GCC_VER}/configure --target=$TARGET --prefix="$PREFIX" \
    --disable-nls --enable-languages=c,c++ --without-headers; \
  make all-gcc; \
  make all-target-libgcc; \
  make install-gcc; \
  make install-target-libgcc; \
  cd $HOME; \
  rm -rf $HOME/src

WORKDIR /work
