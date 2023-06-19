# LLVM Toy RISC-V Backend

This repository is used to show how to build a RISC-V backend step by step. each
commit is used to implement a minimum basic function, and try to avoid
submitting a large amount of code at one time, so that it is easier to
understand each line of code

## References

1. https://jonathan2251.github.io/lbd/
2. https://github.com/rhysd/toy-riscv-backend

## Getting started

### build riscv64-unknown-linux-gnu-gcc

```
$> git clone https://github.com/riscv-collab/riscv-gnu-toolchain
$> cd /path/to/riscv-gnu-toolchain
$> configure --prefix=/opt/gcc-riscv --with-arch=rv64gc --with-abi=lp64d --enable-multilib
$> make linux
```

### build clang

```
$> cd /path/to/llvm-toy/toy
$> ./clang_build.sh
```

### build Toy backend and test

```
$> cd /path/to/llvm-toy/toy
$> make llvm
$> make 
$> make run
$> make run BINARY=1
$> make run GCC=1
```

You may want to checkout every commit I made, and play with the Toy backend step
by step.

### documentation

https://pages.dogdog.run/toolchain/llvm_toy_riscv_backend.html
