all:build

DEBUG ?=

EXECUTABLES = clang clang++ lld ninja cmake
K := $(foreach exec,$(EXECUTABLES), $(if $(shell command -v $(exec)	\
        2>/dev/null),some string,$(error "$(exec) not found")))

BUILD_SRC := $(shell find ./ -name "build_*.c" -printf '%P\n')
BUILD_BC := $(patsubst %.c,%.bc,${BUILD_SRC})
BUILD_LL := $(patsubst %.c,%.ll,${BUILD_SRC})
BUILD_ASM := $(patsubst %.c,%.s,${BUILD_SRC})

RUN_SRC := $(shell find ./ -name "run_*.c" -printf '%P\n')
RUN_ELF := $(patsubst %.c,%.elf,${RUN_SRC})
TEST_APP := $(patsubst %, test_%,${RUN_ELF})

${BUILD_ASM}:llvm

%.bc:%.c
	clang $< -c -fno-stack-protector -emit-llvm -O0 -o $@ && llvm-dis $@

%.s:%.bc
	../build/bin/llc $< -march=toy -o $@ ${DEBUG}

run_%.elf:build_%.s run_%.c
	riscv64-linux-gnu-gcc $^ -O0 -o $@ -static

${TEST_APP}:test_%:%
	qemu-riscv64 $<

${BUILD_ASM}:FORCE

build:${BUILD_ASM}
run:${TEST_APP}

clean:
	rm -f ${BUILD_ASM} ${BUILD_BC} ${BUILD_LL} ${RUN_ELF}

.PRECIOUS: %.bc

FORCE:

llvm:FORCE
	./toy_build.sh
