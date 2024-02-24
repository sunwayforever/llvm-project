#!/bin/bash
mkdir ../build_tooy
pushd ../build_tooy
cmake -DCMAKE_BUILD_TYPE=Release \
      -DLLVM_ENABLE_PROJECTS="mlir" \
      -DLLVM_TARGETS_TO_BUILD="Native;" \
      -DCMAKE_INSTALL_PREFIX="/opt/llvm-mlir" \
      -DLLVM_BUILD_EXAMPLES=ON \
      -DLLVM_BUILD_TESTS=False \
      -DLLVM_ENABLE_LLD=True \
      --log-level=status \
      -DLLVM_PARALLEL_LINK_JOBS=1 -G "Ninja" ../llvm
ninja
popd
