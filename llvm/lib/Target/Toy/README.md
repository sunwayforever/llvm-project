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
$> configure --prefix=/opt/riscv --with-arch=rv64gc --with-abi=lp64d --enable-multilib
$> make linux
```

### build clang

```
$> cd /path/to/llvm-toy
$> git checkout 0d9b36ce7d4d9e8c5bd73b73fb03b462b35539e2
$> mkdir build_clang && cd build_clang
$> cmake -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_INSTALL_PREFIX="/opt/llvm-riscv" \
          -DLLVM_BUILD_TESTS=False \
          -DDEFAULT_SYSROOT="/opt/riscv/sysroot" \
          -DLLVM_DEFAULT_TARGET_TRIPLE="riscv64-unknown-linux-gnu" \
          -DLLVM_TARGETS_TO_BUILD=RISCV -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_LLD=True \
          -DLLVM_OPTIMIZED_TABLEGEN=On -DLLVM_PARALLEL_LINK_JOBS=1 -G "Ninja" ../llvm
$> ninja && ninja install
```

### build Toy backend and test

```
$> cd /path/to/llvm-toy
$> cd toy
$> make llvm
$> make 
$> make run
```

You may want to checkout every commit I made, and play with the Toy backend step
by step.

