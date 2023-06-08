// 2023-05-22 15:13
#ifndef TOY_INST_PRINTER_H
#define TOY_INST_PRINTER_H

#include <llvm/MC/MCInstPrinter.h>
#include <llvm/MC/MCRegister.h>
#include <llvm/Support/raw_ostream.h>

namespace llvm {
class ToyInstPrinter : public MCInstPrinter {
public:
  ToyInstPrinter(MCAsmInfo const &MAI, MCInstrInfo const &MII,
                 MCRegisterInfo const &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  void printInst(MCInst const *MI, uint64_t Address, StringRef Annot,
                 MCSubtargetInfo const &STI, raw_ostream &O) override;
  void printRegName(raw_ostream &OS, MCRegister reg) const override;
  void printMemOperand(const MCInst *MI, int opNum, raw_ostream &O);
  void printMemStackOperand(const MCInst *MI, int opNum, raw_ostream &O);
  // NOTE: ToyGenAsmWriter.inc 需要调这两个函数
  void printOperand(MCInst const *MI, unsigned OpNum, raw_ostream &O);
  void printOperand(MCInst const *MI, uint64_t _Address, unsigned OpNum,
                    raw_ostream &O) {
    printOperand(MI, OpNum, O);
  }
  // NOTE: ToyGenAsmWriter.inc 中定义
  std::pair<char const *, uint64_t> getMnemonic(MCInst const *MI) override;
  void printInstruction(MCInst const *MI, uint64_t Address, raw_ostream &OS);
  static char const *getRegisterName(MCRegister reg);
  bool printAliasInstr(MCInst const *MI, uint64_t Address, raw_ostream &OS);
};

} // namespace llvm

#endif // TOY_INST_PRINTER_H
