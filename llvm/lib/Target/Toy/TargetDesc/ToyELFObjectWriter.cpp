// 2023-06-12 16:50
#include "ToyELFObjectWriter.h"
#include "ToyFixupKinds.h"
#include "llvm/MC/MCContext.h"

using namespace llvm;

unsigned ToyELFObjectWriter::getRelocType(MCContext &Ctx, const MCValue &Target,
                                          const MCFixup &Fixup,
                                          bool IsPCRel) const {
  unsigned Kind = Fixup.getTargetKind();
  switch (Kind) {
  default:
    Ctx.reportError(Fixup.getLoc(), "unsupported relocation type");
    return ELF::R_RISCV_NONE;
  case Toy::fixup_riscv_hi20:
    return ELF::R_RISCV_HI20;
  case Toy::fixup_riscv_lo12_i:
    return ELF::R_RISCV_LO12_I;
  case Toy::fixup_riscv_jal:
    return ELF::R_RISCV_JAL;
  case Toy::fixup_riscv_branch:
    return ELF::R_RISCV_BRANCH;
  }
  return ELF::R_RISCV_NONE;
}
