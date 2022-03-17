; RUN: llc -mtriple=riscv32 -mattr=+d -verify-machineinstrs < %s \
; RUN:   | FileCheck -check-prefix=RV32IFD %s
; XFAIL: *

declare double @llvm.floor.f64(double)

; Generates a bitcast to the illegal i64 type while lowering a libcall to
; ffloor.

define double @foo(double %a) nounwind {
  %1 = call double @llvm.floor.f64(double %a)
  ret double %1
}
