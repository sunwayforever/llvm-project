all:build

CLANG ?= clang
GCC ?= riscv64-unknown-linux-gnu-gcc
DEBUG ?=

EXECUTABLES = ${CLANG} clang++ lld ninja cmake ${GCC} qemu-riscv32
K := $(foreach exec,$(EXECUTABLES), $(if $(shell command -v $(exec)	\
        2>/dev/null),some string,$(error "$(exec) not found")))

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_BC := $(patsubst %.c,%.bc,${TEST_SRC})
TEST_LL := $(patsubst %.c,%.ll,${TEST_SRC})
TEST_ASM := $(patsubst %.c,%.s,${TEST_SRC})

CHECK_ASM := $(patsubst %.c, check_%,${TEST_SRC})

RUN_ELF := $(patsubst %.c,%.elf,${TEST_SRC})
RUN_APP := $(patsubst %.elf, run_%,${RUN_ELF})

${TEST_ASM}:llvm

%.bc:%.c
	${CLANG} $< -c -emit-llvm -O0 -o $@ && llvm-dis $@

%.s:%.bc
	../build/bin/llc $< -march=toy -o $@ ${DEBUG}

# /opt/riscv32-ilp32/bin/riscv32-unknown-linux-gnu-gcc -march=rv32im -mabi=ilp32 $^ -O0 -o $@ -static
%.elf:%.s
	${GCC} -march=rv32g -mabi=ilp32 $^ -O0 -o $@ -static

${CHECK_ASM}:check_%:%.s
	cat $<

${RUN_APP}:run_%:%.elf
	qemu-riscv32 $<

${BUILD_ASM}:FORCE

build:${TEST_ASM}
run:${RUN_APP}

clean:
	rm -f ${TEST_ASM} ${TEST_BC} ${TEST_LL} ${RUN_ELF}

.PRECIOUS: %.bc

FORCE:

llvm:FORCE
	./toy_build.sh
