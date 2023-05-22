// 2023-05-22 15:15
#include "ToyInstPrinter.h"
#include "llvm/MC/MCInst.h"

using namespace llvm;

#define PRINT_ALIAS_INSTR
#include "ToyGenAsmWriter.inc"

void ToyInstPrinter::printInst(MCInst const *MI, uint64_t Address,
                               StringRef Annot, MCSubtargetInfo const &STI,
                               raw_ostream &O) {}

void ToyInstPrinter::printRegName(raw_ostream &OS, MCRegister reg) const {}

void ToyInstPrinter::printOperand(MCInst const *MI, unsigned OpNo,
                                  raw_ostream &OS) {}

void ToyInstPrinter::printOperand(MCInst const *MI, uint64_t _Address,
                                  unsigned OpNum, raw_ostream &O) {}
