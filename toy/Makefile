all:build

CLANG_PREFIX ?= ../build_clang/bin/
GCC_PREFIX ?= /opt/gcc-riscv/bin/riscv64-unknown-linux-gnu-

DEBUG ?=
BINARY ?=
GCC ?=

EXECUTABLES = ${CLANG_PREFIX}clang lld ninja cmake ${GCC_PREFIX}gcc qemu-riscv32
K := $(foreach exec,$(EXECUTABLES), $(if $(shell command -v $(exec)	\
        2>/dev/null),some string,$(error "$(exec) not found")))

MAIN_SRC := $(shell find ./ -name "*.main.c" -printf '%P\n')
MAIN_ELF := $(patsubst %.main.c,%.elf,${MAIN_SRC})
${MAIN_ELF}:%.elf:%.main.c

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_SRC := $(filter-out ${MAIN_SRC}, ${TEST_SRC})
ifeq (${GCC}, 1)
TEST_SRC := $(filter-out builtin.c, ${TEST_SRC})
endif
TEST_BC := $(patsubst %.c,%.bc,${TEST_SRC})
TEST_LL := $(patsubst %.c,%.ll,${TEST_SRC})
TEST_ASM := $(patsubst %.c,%.s,${TEST_SRC})
TEST_OBJ := $(patsubst %.c,%.o,${TEST_SRC})
RUN_ELF := $(patsubst %.c,%.elf,${TEST_SRC})

RUN_APP := $(patsubst %.elf, run_%,${RUN_ELF})

${TEST_ASM}:llvm
${TEST_OBJ}:llvm

${TEST_BC}:%.bc:%.c
	${CLANG_PREFIX}clang --target=riscv64-unknown-gnu -march=rv32g $< -c -emit-llvm -O0 -o $@ && ${CLANG_PREFIX}llvm-dis $@

ifeq (${GCC}, 1)
${TEST_ASM}:%.s:%.c
	${GCC_PREFIX}gcc -march=rv32g -mabi=ilp32d $< -O3 -o $@ -S
else
${TEST_ASM}:%.s:%.bc
	../build/bin/llc $< -march=toy -o $@ ${DEBUG}
endif

ifeq (${GCC}, 1)
${TEST_OBJ}:%.o:%.c
	${GCC_PREFIX}gcc -march=rv32g -mabi=ilp32d $< -O3 -o $@ -c
else
${TEST_OBJ}:%.o:%.bc
	../build/bin/llc $< -march=toy -filetype=obj -o $@ ${DEBUG}
endif

ifeq (${BINARY}, 1)
${RUN_ELF}:%.elf:%.o
else
${RUN_ELF}:%.elf:%.s
endif
	${GCC_PREFIX}gcc -march=rv32g -mabi=ilp32d $^ -O0 -o $@ -static -lm

${RUN_APP}:run_%:%.elf
	qemu-riscv32 $<

${TEST_ASM} ${TEST_OBJ}:FORCE

ifeq (${BINARY}, 1)
build:${TEST_OBJ}
else
build:${TEST_ASM}
endif

run:${RUN_APP}

clean:
	rm -f ${TEST_ASM} ${TEST_BC} ${TEST_LL} ${TEST_OBJ} ${RUN_ELF}

.PRECIOUS: %.bc

test:
	make run && make run BINARY=1 && make run GCC=1

FORCE:

llvm:FORCE
	./toy_build.sh
