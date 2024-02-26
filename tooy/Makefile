all:build

TOOY_PREFIX ?= ../build_tooy/bin/tooyc
LLVM_PREFIX ?= ../build_llvm/bin/

TEST_TOOY_SRC := $(shell find ./ -name "*.toy" -printf '%P\n')
TEST_TOOY_AST := $(patsubst %.toy,%.ast,${TEST_TOOY_SRC})
TEST_TOOY_MLIR := $(patsubst %.toy,%.0.mlir,${TEST_TOOY_SRC})
TEST_TOOY_MLIR_AFFINE := $(patsubst %.toy,%.1.mlir,${TEST_TOOY_SRC})
TEST_TOOY_MLIR_LLVM := $(patsubst %.toy,%.2.mlir,${TEST_TOOY_SRC})

TEST_TOOY_LL := $(patsubst %.toy,%.ll,${TEST_TOOY_SRC})
TEST_TOOY_ELF := $(patsubst %.toy,run_%,${TEST_TOOY_SRC})

${TEST_TOOY_AST}:%.ast:FORCE
${TEST_TOOY_AST}:%.ast:%.toy
	${TOOY_PREFIX} $< -emit=ast 2> $@ ; cat $@

${TEST_TOOY_MLIR}:%.0.mlir:FORCE
${TEST_TOOY_MLIR}:%.0.mlir:%.toy
	${TOOY_PREFIX} $< -emit=mlir 2> $@ ; cat $@

${TEST_TOOY_MLIR_AFFINE}:%.1.mlir:FORCE
${TEST_TOOY_MLIR_AFFINE}:%.1.mlir:%.toy
	${TOOY_PREFIX} $< -emit=mlir-affine 2> $@ ; cat $@

${TEST_TOOY_MLIR_LLVM}:%.2.mlir:FORCE
${TEST_TOOY_MLIR_LLVM}:%.2.mlir:%.toy
	${TOOY_PREFIX} $< -emit=mlir-llvm 2> $@ ; cat $@

${TEST_TOOY_LL}:%.ll:FORCE
${TEST_TOOY_LL}:%.ll:%.toy
	${TOOY_PREFIX} $< -emit=llvm 2> $@ ; cat $@

${TEST_TOOY_ELF}:run_%:%.toy
	${TOOY_PREFIX} $< -emit=jit

build:${TEST_TOOY_LL}

clean:
	rm -f ${TEST_TOOY_AST} ${TEST_TOOY_MLIR} ${TEST_TOOY_MLIR_AFFINE} ${TEST_TOOY_MLIR_LLVM} ${TEST_TOOY_LL}

PHONY: FORCE

FORCE:

tooy:FORCE
	./tooy_build.sh
