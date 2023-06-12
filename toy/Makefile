all:build

# NOTE: to build riscv64-unknown-linux-gnu-gcc
# $> configure --prefix=/opt/riscv --with-arch=rv64gc --with-abi=lp64d --enable-multilib
# $> make linux

# NOTE: to build clang
# $> cmake -DCMAKE_BUILD_TYPE=Release \
#          -DCMAKE_INSTALL_PREFIX="/opt/llvm-riscv" \
#          -DLLVM_BUILD_TESTS=False \
#          -DDEFAULT_SYSROOT="/opt/riscv/sysroot" \
#          -DLLVM_DEFAULT_TARGET_TRIPLE="riscv64-unknown-linux-gnu" \
#          -DLLVM_TARGETS_TO_BUILD=RISCV -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_LLD=True \
#          -DLLVM_OPTIMIZED_TABLEGEN=On -DLLVM_PARALLEL_LINK_JOBS=1 -G "Ninja" ../llvm
# $> ninja && ninja install

CLANG_PREFIX ?= /opt/llvm-riscv/bin/
GCC_PREFIX ?= riscv64-unknown-linux-gnu-
DEBUG ?=

EXECUTABLES = ${CLANG_PREFIX}clang lld ninja cmake ${GCC_PREFIX}gcc qemu-riscv32
K := $(foreach exec,$(EXECUTABLES), $(if $(shell command -v $(exec)	\
        2>/dev/null),some string,$(error "$(exec) not found")))

MAIN_SRC := $(shell find ./ -name "*.main.c" -printf '%P\n')
MAIN_ELF := $(patsubst %.main.c,%.elf,${MAIN_SRC})
${MAIN_ELF}:%.elf:%.main.c

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_SRC := $(filter-out ${MAIN_SRC}, ${TEST_SRC})
TEST_BC := $(patsubst %.c,%.bc,${TEST_SRC})
TEST_LL := $(patsubst %.c,%.ll,${TEST_SRC})
TEST_ASM := $(patsubst %.c,%.s,${TEST_SRC})
TEST_OBJ := $(patsubst %.c,%.o,${TEST_SRC})

CHECK_ASM := $(patsubst %.c, check_%,${TEST_SRC})

RUN_ELF := $(patsubst %.c,%.elf,${TEST_SRC})
RUN_APP := $(patsubst %.elf, run_%,${RUN_ELF})

${TEST_ASM}:llvm
${TEST_OBJ}:llvm

%.bc:%.c
	${CLANG_PREFIX}clang --target=riscv64-unknown-gnu -march=rv32g $< -c -emit-llvm -O0 -o $@ && ${CLANG_PREFIX}llvm-dis $@

%.s:%.bc
	../build/bin/llc $< -march=toy -o $@ ${DEBUG}

${TEST_OBJ}:%.o:%.bc
	../build/bin/llc $< -march=toy -filetype=obj -o $@ ${DEBUG}

${RUN_ELF}:%.elf:%.s
	${GCC_PREFIX}gcc -march=rv32g -mabi=ilp32d $^ -O0 -o $@ -static

${CHECK_ASM}:check_%:%.s
	cat $<

${RUN_APP}:run_%:%.elf
	qemu-riscv32 $<

${BUILD_ASM}:FORCE

build:${TEST_ASM}
run:${RUN_APP}

clean:
	rm -f ${TEST_ASM} ${TEST_BC} ${TEST_LL} ${TEST_OBJ} ${RUN_ELF}

.PRECIOUS: %.bc

FORCE:

llvm:FORCE
	./toy_build.sh
