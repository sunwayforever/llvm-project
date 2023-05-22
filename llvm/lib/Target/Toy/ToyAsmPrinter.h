// 2023-05-22 17:00
#ifndef TOY_ASM_PRINTER_H
#define TOY_ASM_PRINTER_H

#include "ToyTargetMachine.h"
#include <llvm/CodeGen/AsmPrinter.h>
#include <llvm/MC/MCStreamer.h>

namespace llvm {
class ToyAsmPrinter : public AsmPrinter {
public:
  ToyAsmPrinter(TargetMachine &TM, std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  virtual StringRef getPassName() const override { return "toy asm printer"; }
};
} // namespace llvm

#endif // TOY_ASM_PRINTER_H
