all:build

TOOY_PREFIX ?= ../build_tooy/bin/tooyc
LLVM_PREFIX ?= ../build_llvm/bin/

TEST_TOOY_SRC := $(shell find ./ -name "*.toy" -printf '%P\n')
TEST_TOOY_AST := $(patsubst %.toy,%.ast,${TEST_TOOY_SRC})
TEST_TOOY_MLIR := $(patsubst %.toy,%.mlir,${TEST_TOOY_SRC})
TEST_TOOY_LL := $(patsubst %.toy,%.ll,${TEST_TOOY_SRC})

${TEST_TOOY_AST}:%.ast:FORCE
${TEST_TOOY_AST}:%.ast:%.toy
	${TOOY_PREFIX} $< -emit=ast 2> $@

${TEST_TOOY_MLIR}:%.mlir:FORCE
${TEST_TOOY_MLIR}:%.mlir:%.toy
	${TOOY_PREFIX} $< -emit=mlir 2> $@

${TEST_TOOY_LL}:%.ll:FORCE
${TEST_TOOY_LL}:%.ll:%.toy
	${TOOY_PREFIX} $< -emit=llvm 2> $@

build:${TEST_TOOY_LL}

clean:
	rm -f ${TEST_TOOY_AST} ${TEST_TOOY_MLIR} ${TEST_TOOY_LL}

PHONY: FORCE

FORCE:

tooy:FORCE
	./tooy_build.sh
