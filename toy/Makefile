all:build

DEBUG ?=

EXECUTABLES = clang clang++ lld ninja cmake riscv32-unknown-linux-gnu-gcc qemu-riscv32
K := $(foreach exec,$(EXECUTABLES), $(if $(shell command -v $(exec)	\
        2>/dev/null),some string,$(error "$(exec) not found")))

BUILD_SRC := $(shell find ./ -name "build_*.c" -printf '%P\n')
BUILD_BC := $(patsubst %.c,%.bc,${BUILD_SRC})
BUILD_LL := $(patsubst %.c,%.ll,${BUILD_SRC})
BUILD_ASM := $(patsubst %.c,%.s,${BUILD_SRC})

CHECK_ASM := $(patsubst build_%.c, check_%,${BUILD_SRC})

RUN_SRC := $(shell find ./ -name "run_*.c" -printf '%P\n')
RUN_ELF := $(patsubst run_%.c,%.elf,${RUN_SRC})
RUN_APP := $(patsubst %.elf, run_%,${RUN_ELF})

${BUILD_ASM}:llvm

%.bc:%.c
	clang $< -c -fno-stack-protector -emit-llvm -O0 -o $@ && llvm-dis $@

%.s:%.bc
	../build/bin/llc $< -march=toy -o $@ ${DEBUG}

%.elf:build_%.s run_%.c
	riscv32-unknown-linux-gnu-gcc $^ -O0 -o $@ -static

${CHECK_ASM}:check_%:build_%.s
	cat $<

${RUN_APP}:run_%:%.elf
	qemu-riscv32 $<

${BUILD_ASM}:FORCE

build:${BUILD_ASM}
run:${RUN_APP}

clean:
	rm -f ${BUILD_ASM} ${BUILD_BC} ${BUILD_LL} ${RUN_ELF}

.PRECIOUS: %.bc

FORCE:

llvm:FORCE
	./toy_build.sh
