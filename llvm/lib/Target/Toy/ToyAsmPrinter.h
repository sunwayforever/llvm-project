// 2023-05-22 17:00
#ifndef TOY_ASM_PRINTER_H
#define TOY_ASM_PRINTER_H

#include "ToyMCInstLower.h"
#include "ToyTargetMachine.h"
#include <llvm/CodeGen/AsmPrinter.h>
#include <llvm/MC/MCStreamer.h>

namespace llvm {
class MachineInstr;
class ToyAsmPrinter : public AsmPrinter {
private:
  ToyMCInstLower MCInstLowering;

public:
  ToyAsmPrinter(TargetMachine &TM, std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)), MCInstLowering(*this) {}

  virtual StringRef getPassName() const override { return "toy asm printer"; }
  void emitInstruction(const MachineInstr *MI) override;
  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                   const MachineInstr *MI);
  bool lowerOperand(const MachineOperand &MO, MCOperand &MCOp) const {
    MCOp = MCInstLowering.LowerOperand(MO);
    return true;
  }
};
} // namespace llvm

#endif // TOY_ASM_PRINTER_H
