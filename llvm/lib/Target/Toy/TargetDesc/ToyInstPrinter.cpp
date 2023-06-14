// 2023-05-22 15:15
#include "ToyInstPrinter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"

#define DEBUG_TYPE "toy instr printer"
#include <llvm/Support/Debug.h>

using namespace llvm;

#define PRINT_ALIAS_INSTR
#include "ToyGenAsmWriter.inc"

void ToyInstPrinter::printInst(MCInst const *MI, uint64_t Address,
                               StringRef Annot, MCSubtargetInfo const &STI,
                               raw_ostream &O) {
  printInstruction(MI, Address, O);
}

void ToyInstPrinter::printRegName(raw_ostream &O, MCRegister reg) const {
  O << StringRef(getRegisterName(reg)).lower();
}

void ToyInstPrinter::printOperand(MCInst const *MI, unsigned OpNum,
                                  raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNum);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  Op.getExpr()->print(O, &MAI, true);
}
