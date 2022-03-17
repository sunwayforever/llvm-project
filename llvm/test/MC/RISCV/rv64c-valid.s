# RUN: llvm-mc -triple=riscv64 -mattr=+c -show-encoding < %s \
# RUN:     | FileCheck -check-prefixes=CHECK,CHECK-INST %s
# RUN: llvm-mc -filetype=obj -triple riscv64 -mattr=+c < %s \
# RUN:     | llvm-objdump -mattr=+c -d - \
# RUN:     | FileCheck -check-prefix=CHECK-INST %s
# RUN: not llvm-mc -triple riscv64 \
# RUN:     -show-encoding < %s 2>&1 \
# RUN:     | FileCheck -check-prefixes=CHECK-NO-EXT %s
# RUN: not llvm-mc -triple riscv32 -mattr=+c\
# RUN:     -show-encoding < %s 2>&1 \
# RUN:     | FileCheck -check-prefixes=CHECK-NO-EXT %s

# TODO: more exhaustive testing of immediate encoding.

# CHECK-INST: c.ldsp  ra, 0(sp)
# CHECK: encoding: [0x82,0x60]
# CHECK-NO-EXT:  error: instruction use requires an option to be enabled
c.ldsp  ra, 0(sp)
# CHECK-INST: c.sdsp  ra, 504(sp)
# CHECK: encoding: [0x86,0xff]
# CHECK-NO-EXT:  error: instruction use requires an option to be enabled
c.sdsp  ra, 504(sp)
# CHECK-INST: c.ld    a4, 0(a3)
# CHECK: encoding: [0x98,0x62]
# CHECK-NO-EXT:  error: instruction use requires an option to be enabled
c.ld    a4, 0(a3)
# CHECK-INST: c.sd    a5, 248(a3)
# CHECK: encoding: [0xfc,0xfe]
# CHECK-NO-EXT:  error: instruction use requires an option to be enabled
c.sd    a5, 248(a3)
