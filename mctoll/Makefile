all:build

CLANG ?= ../build/bin/clang
LLVM_DIS ?= ../build/bin/llvm-dis
LLVM_OPT ?= ../build/bin/opt
MCTOLL ?= ../build/bin/llvm-mctoll

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_LL := $(patsubst %.c,%.ll,${TEST_SRC})
TEST_ASM := $(patsubst %.c,%.s,${TEST_SRC})
TEST_OPT_LL := $(patsubst %.c,%.opt.ll,${TEST_SRC})
TEST_OPT_ASM := $(patsubst %.c,%.opt.s,${TEST_SRC})

TEST_X86_ASM := $(patsubst %.c,%.x86.s,${TEST_SRC})
TEST_X86_LL := $(patsubst %.c,%.x86.ll,${TEST_SRC})
TEST_X86_ELF := $(patsubst %.c,%.x86.elf,${TEST_SRC})
TEST_ELF := $(patsubst %.c,%.elf,${TEST_SRC})

RUN_APP := $(patsubst %.elf, run_%,${TEST_ELF})

${TEST_X86_ELF}:%.x86.elf:%.c
	${CLANG} $< -O0 -o $@

${TEST_X86_LL}:%.x86.ll:%.c
	${CLANG} $< -c -emit-llvm -O0 -o /tmp/1.bc && \
		${LLVM_DIS} /tmp/1.bc -o $@

${TEST_X86_ASM}:%.x86.s:%.c
	${CLANG} $< -o $@ -S

${TEST_ASM}:%.s:%.ll
	${CLANG} -target riscv64-unknown-linux-gnu $< -o $@ -S

${TEST_LL}:%.ll:%.x86.elf
	${MCTOLL} -d -I /usr/include/stdio.h $< -o $@

${TEST_OPT_LL}:%.opt.ll:%.ll
	${LLVM_OPT} -O2 $< -o /tmp/1.bc && \
		${LLVM_DIS} /tmp/1.bc -o $@

${TEST_OPT_ASM}:%.opt.s:%.opt.ll
	${CLANG} -target riscv64-unknown-linux-gnu $< -o $@ -S

${TEST_ELF}:%.elf:%.ll
	${CLANG} -target riscv64-unknown-linux-gnu $< -o $@ -static

${RUN_APP}:run_%:%.elf
	qemu-riscv64 ./$<

${TEST_LL} ${TEST_X86_ELF}:FORCE

build:${TEST_LL} ${TEST_X86_LL} ${TEST_X86_ASM} ${TEST_OPT_LL} ${TEST_OPT_ASM}

run:${RUN_APP}

clean:
	rm -f ${TEST_LL} ${TEST_ELF} ${TEST_X86_ELF} ${TEST_OPT_LL} ${TEST_X86_LL} \
		${TEST_X86_ASM} ${TEST_ASM} ${TEST_OPT_ASM}

FORCE:
