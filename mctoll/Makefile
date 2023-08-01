all:build

CLANG ?= ../build/bin/clang
MCTOLL ?= ../build/bin/llvm-mctoll

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_LL := $(patsubst %.c,%.ll,${TEST_SRC})
TEST_X86_ELF := $(patsubst %.c,%.x86.elf,${TEST_SRC})
TEST_ELF := $(patsubst %.c,%.elf,${TEST_SRC})

RUN_APP := $(patsubst %.elf, run_%,${TEST_ELF})

${TEST_X86_ELF}:%.x86.elf:%.c
	${CLANG} $< -O0 -o $@

${TEST_LL}:%.ll:%.x86.elf
	${MCTOLL} -d -I /usr/include/stdio.h $< -o $@

${TEST_ELF}:%.elf:%.ll
	${CLANG} -target riscv64-unknown-linux-gnu $< -o $@ -static

${RUN_APP}:run_%:%.elf
	qemu-riscv64 ./$<

${TEST_LL} ${TEST_X86_ELF}:FORCE

build:${TEST_LL}

run:${RUN_APP}

clean:
	rm -f ${TEST_LL} ${TEST_ELF} ${TEST_X86_ELF}


FORCE:
