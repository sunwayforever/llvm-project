#!/bin/bash
if [[ -d ../build ]]; then
    ninja -C ../build llc
else
    mkdir ../build
    pushd ../build
    cmake -DCMAKE_BUILD_TYPE=Debug \
          -DCMAKE_CXX_COMPILER=clang++ \
          -DCMAKE_C_COMPILER=clang \
          -DLLVM_TARGETS_TO_BUILD=Toy \
          -DLLVM_USE_LINKER=lld \
          -DLLVM_OPTIMIZED_TABLEGEN=OFF \
          -DLLVM_ENABLE_DUMP=ON \
          -DLLVM_PARALLEL_LINK_JOBS=1 \
          -G "Ninja" ../llvm
    ninja llc
    popd
fi
