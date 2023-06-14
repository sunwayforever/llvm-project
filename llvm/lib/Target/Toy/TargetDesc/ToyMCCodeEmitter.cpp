// 2023-06-12 16:06
#include "ToyMCCodeEmitter.h"
#include "ToyFixupKinds.h"
#include "ToyMCExpr.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "toy code emitter"

using namespace llvm;

void ToyMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  LLVM_DEBUG(errs() << MI);
  uint32_t Binary = getBinaryCodeForInstr(MI, Fixups, STI);

  for (int i = 0; i < 4; ++i) {
    unsigned Shift = i * 8;
    OS << (char)((Binary >> Shift) & 0xff);
  }
}

unsigned ToyMCCodeEmitter::getMachineOpValue(const MCInst &MI,
                                             const MCOperand &MO,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  if (MO.isReg()) {
    unsigned Reg = MO.getReg();
    unsigned RegNo = Ctx.getRegisterInfo()->getEncodingValue(Reg);
    return RegNo;
  } else if (MO.isImm()) {
    return static_cast<unsigned>(MO.getImm());
  }
  assert(MO.isExpr());
  const MCExpr *Expr = MO.getExpr();
  const ToyMCExpr *ToyExpr = cast<ToyMCExpr>(Expr);
  Toy::Fixups FixupKind = Toy::Fixups::fixup_riscv_invalid;
  switch (ToyExpr->getKind()) {
  case ToyMCExpr::TEK_HI:
    FixupKind = Toy::Fixups::fixup_riscv_hi20;
    break;
  case ToyMCExpr::TEK_LO:
    FixupKind = Toy::Fixups::fixup_riscv_lo12_i;
    break;
  case ToyMCExpr::TEK_JAL:
    FixupKind = Toy::Fixups::fixup_riscv_jal;
    break;
  case ToyMCExpr::TEK_BRANCH:
    FixupKind = Toy::Fixups::fixup_riscv_branch;
    break;
  }
  Fixups.push_back(MCFixup::create(0, Expr, MCFixupKind(FixupKind)));
  return 0;
}

#include "ToyGenMCCodeEmitter.inc"
