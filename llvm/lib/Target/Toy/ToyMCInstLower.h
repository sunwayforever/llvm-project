// 2023-05-24 16:10
#ifndef TOY_MC_INST_LOWER_H
#define TOY_MC_INST_LOWER_H

namespace llvm {
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;
class ToyAsmPrinter;
class ToyMCInstLower {
private:
  ToyAsmPrinter &AsmPrinter;
  MCOperand LowerSymbolOperand(const MachineOperand &MO) const;

public:
  ToyMCInstLower(ToyAsmPrinter &printer) : AsmPrinter(printer){};
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;
  MCOperand LowerOperand(const MachineOperand &MO) const;
};
} // namespace llvm

#endif // TOY_MC_INST_LOWER_H
