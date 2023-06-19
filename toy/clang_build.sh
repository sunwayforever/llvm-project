#!/bin/bash
if [[ -d ../build_clang ]]; then
    ninja -C ../build_clang clang
else
    mkdir ../build_clang
    pushd ../build_clang
    cmake -DCMAKE_BUILD_TYPE=Debug \
          -DCMAKE_INSTALL_PREFIX="/opt/llvm-riscv" \
          -DLLVM_BUILD_TESTS=False \
          -DDEFAULT_SYSROOT="/opt/gcc-riscv/sysroot/" \
          -DLLVM_DEFAULT_TARGET_TRIPLE="riscv64-unknown-linux-gnu" \
          -DLLVM_TARGETS_TO_BUILD=RISCV -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_LLD=True \
          -DLLVM_OPTIMIZED_TABLEGEN=On -DLLVM_PARALLEL_LINK_JOBS=1 -G "Ninja" ../llvm
    ninja clang
    popd
fi
