all:build

# NOTE: 由于 libgcc 等不在 sysroot 中, 需要手动复制
# cd $GNU; cp -a lib/gcc sysroot/lib/
GNU := ${HOME}/program/toolchain/riscv64-unknown-linux-gnu
SYSROOT := ${GNU}/sysroot
CXX_HEADER_PATH := -I${GNU}/riscv64-unknown-linux-gnu/include/c++/13.2.0 \
	-I${GNU}/riscv64-unknown-linux-gnu/include/c++/13.2.0/riscv64-unknown-linux-gnu

CLANG_PREFIX ?= ../build_clang/bin/
LLVM_PREFIX ?= ../build_llvm/bin/
GNU_PREFIX ?= ${GNU}/bin/riscv64-unknown-linux-gnu-
QEMU ?= qemu-riscv64
USE_LD ?= ${GNU_PREFIX}ld
# USE_LD ?= lld

INTEGRATED_AS ?= 1

DEBUG ?=
# DEBUG = --view-dag-combinel-dags
# DEBUG = --view-Legalize-dags
# DEBUG = --view-isel-dags
# DEBUG = --view-sched-dags
# DEBUG = --print-after-all
# DEBUG = --debug
# DEBUG = --debug-pass=Structure
O ?= 2

ATTR ?=

TEST_C_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_CC_SRC += $(shell find ./ -name "*.cc" -printf '%P\n')

TEST_C_BC := $(patsubst %.c,%.bc,${TEST_C_SRC})
TEST_CC_BC += $(patsubst %.cc,%.bc,${TEST_CC_SRC})
TEST_BC := ${TEST_C_BC} ${TEST_CC_BC}

TEST_LL := $(patsubst %.bc,%.ll,${TEST_BC})
TEST_ASM := $(patsubst %.bc,%.s,${TEST_BC})
TEST_OBJ := $(patsubst %.bc,%.o,${TEST_BC})
TEST_DUMP := $(patsubst %.bc,%.dump,${TEST_BC})
TEST_ELF := $(patsubst %.bc,%.elf, ${TEST_BC})
RUN_ELF := $(patsubst %.bc,run_%,${TEST_BC})
DEBUG_ELF := $(patsubst %.bc,debug_%,${TEST_BC})
# ${TEST_ASM}:llvm
# ${TEST_OBJ}:llvm

CPU=sifive-s76

${TEST_BC}:%.bc:FORCE
${TEST_C_BC}:%.bc:%.c
	${CLANG_PREFIX}clang --target=riscv64-unknown-linux-gnu -mcpu=${CPU} --sysroot=${SYSROOT} \
		$< -c -emit-llvm -O${O} -o $@

${TEST_CC_BC}:%.bc:%.cc
	${CLANG_PREFIX}clang++ --target=riscv64-unknown-linux-gnu -mcpu=${CPU} --sysroot=${SYSROOT} \
		${CXX_HEADER_PATH} $< -c -emit-llvm -O${O} -o $@

${TEST_LL}:%.ll:%.bc
	${CLANG_PREFIX}llvm-dis $< -o $@

${TEST_ASM}:%.s:%.bc
	${LLVM_PREFIX}llc $< -mcpu=${CPU} -filetype=asm ${ATTR} -O=${O} -o $@ ${DEBUG}

ifeq (${INTEGRATED_AS}, 1)
${TEST_OBJ}:%.o:%.bc
	${LLVM_PREFIX}llc $< -mcpu=${CPU} -filetype=obj ${ATTR} -O=${O} -o $@ ${DEBUG}
else
${TEST_OBJ}:%.o:%.s
	${GNU_PREFIX}as $< -o $@
endif

${TEST_DUMP}:%.dump:%.o
	${GNU_PREFIX}objdump -dr $< > $@

elf:${TEST_ELF}
${TEST_ELF}:%.elf:%.o
	${CLANG_PREFIX}clang --target=riscv64-unknown-linux-gnu -mcpu=${CPU} \
		--sysroot=${SYSROOT} -fuse-ld=${USE_LD} -rtlib=libgcc $< -o $@ -lstdc++ -static

${RUN_ELF}:run_%:%.elf
	${QEMU} $<

${DEBUG_ELF}:debug_%:%.elf
	./qemu-gdb.sh $<

${TEST_ASM} ${TEST_OBIJ}: FORCE
build: ${TEST_DUMP}

clean:
	rm -f ${TEST_ASM} ${TEST_BC} ${TEST_LL} ${TEST_OBJ} ${TEST_DUMP} ${TEST_ELF}

PRECIOUS: %.bc
PHONY: FORCE llvm clang

FORCE:

llvm: FORCE
	./llvm_build.sh

clang:FORCE
	SYSROOT=${SYSROOT} ./clang_build.sh

test: LLvm
	${CLANG_PREFIX}/llvm-lit -v .
