// 2023-05-24 16:14
#include "ToyMCInstLower.h"
#include "TargetDesc/ToyBaseInfo.h"
#include "TargetDesc/ToyMCExpr.h"
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
  ToyMCExpr::ToyExprKind TargetKind = ToyMCExpr::TEK_NONE;
  const MCSymbol *Symbol;

  switch (MO.getTargetFlags()) {
  default:
    llvm_unreachable("Invalid target flag!");
  case ToyII::MO_NO_FLAG:
    break;
  case ToyII::MO_HI:
    TargetKind = ToyMCExpr::TEK_HI;
    break;
  case ToyII::MO_LO:
    TargetKind = ToyMCExpr::TEK_LO;
    break;
  }

  switch (MO.getType()) {
  case MachineOperand::MO_GlobalAddress:
    Symbol = AsmPrinter.getSymbol(MO.getGlobal());
    break;
  default:
    llvm_unreachable("<unknown operand type>");
  }

  const MCExpr *Expr =
      MCSymbolRefExpr::create(Symbol, MCSymbolRefExpr::VK_None, Ctx);
  Expr = new ToyMCExpr(TargetKind, Expr);

  return MCOperand::createExpr(Expr);
}

MCOperand ToyMCInstLower::LowerOperand(const MachineOperand &MO) const {
  switch (MO.getType()) {
  default:
    llvm_unreachable("unknown operand type");
  case MachineOperand::MO_Register:
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm());
  case MachineOperand::MO_GlobalAddress:
    return LowerSymbolOperand(MO);
  }

  return MCOperand();
}
