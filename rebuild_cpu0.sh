#!/bin/bash
rm -rf build && mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang   -DLLVM_TARGETS_TO_BUILD=Cpu0 -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_LLD=True -DLLVM_OPTIMIZED_TABLEGEN=On -DLLVM_PARALLEL_LINK_JOBS=1 -G "Ninja" ../llvm
ninja
popd
