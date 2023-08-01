#!/bin/bash
if [[ -d ../build ]]; then
    ninja -C ../build
else
    mkdir ../build
    pushd ../build
    cmake -S ../llvm -G "Ninja" \
          -DLLVM_TARGETS_TO_BUILD="X86;ARM;RISCV"  \
          -DLLVM_ENABLE_PROJECTS="clang;lld" \
          -DLLVM_ENABLE_ASSERTIONS=true      \
          -DCLANG_DEFAULT_PIE_ON_LINUX=OFF   \
          -DCMAKE_BUILD_TYPE=Release
    ninja
    popd
fi
