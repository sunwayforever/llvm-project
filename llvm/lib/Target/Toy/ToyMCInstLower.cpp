// 2023-05-24 16:14
#include "ToyMCInstLower.h"
#include "ToyAsmPrinter.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/MC/MCInst.h"

using namespace llvm;

void ToyMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    MCOperand MCOp = LowerOperand(MO);
    if (MCOp.isValid()) {
      OutMI.addOperand(MCOp);
    }
  }
}

MCOperand ToyMCInstLower::LowerSymbolOperand(const MachineOperand &MO) const {
  MCContext &Ctx = AsmPrinter.OutContext;
  const MCSymbol *Symbol;

  switch (MO.getType()) {
  case MachineOperand::MO_GlobalAddress:
    Symbol = AsmPrinter.getSymbol(MO.getGlobal());
    break;
  case MachineOperand::MO_MachineBasicBlock:
    Symbol = MO.getMBB()->getSymbol();
    break;
  default:
    llvm_unreachable("<unknown operand type>");
  }

  const MCExpr *Expr =
      MCSymbolRefExpr::create(Symbol, MCSymbolRefExpr::VK_None, Ctx);

  return MCOperand::createExpr(Expr);
}

MCOperand ToyMCInstLower::LowerOperand(const MachineOperand &MO) const {
  switch (MO.getType()) {
  case MachineOperand::MO_Register:
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm());
  case MachineOperand::MO_GlobalAddress:
  case MachineOperand::MO_MachineBasicBlock:
    return LowerSymbolOperand(MO);
  }

  return MCOperand();
}
