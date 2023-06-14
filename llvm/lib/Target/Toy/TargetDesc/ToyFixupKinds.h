// 2023-06-14 11:27
#ifndef TOY_FIXUP_KINDS_H
#define TOY_FIXUP_KINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace Toy {
enum Fixups {
  // 20-bit fixup corresponding to %hi(foo) for instructions like lui
  fixup_riscv_hi20 = FirstTargetFixupKind,
  // 12-bit fixup corresponding to %lo(foo) for instructions like addi
  fixup_riscv_lo12_i,
  fixup_riscv_invalid,
  NumTargetFixupKinds = fixup_riscv_invalid - FirstTargetFixupKind
};
}
} // namespace llvm

#endif // TOY_FIXUP_KINDS_H
