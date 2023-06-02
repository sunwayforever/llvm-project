// 2023-05-24 10:18
#include "ToyRegisterInfo.h"
#include "TargetDesc/ToyTargetDesc.h"
#include "ToySubtarget.h"
#include <llvm/CodeGen/MachineFrameInfo.h>

#define DEBUG_TYPE "toy register info"

#define GET_REGINFO_TARGET_DESC
#include "ToyGenRegisterInfo.inc"

using namespace llvm;

ToyRegisterInfo::ToyRegisterInfo(ToySubtarget const &ST, unsigned HwMode)
    : ToyGenRegisterInfo(0, 0, 0, 0, HwMode) {}

bool ToyRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  MachineInstr &MI = *II;
  LLVM_DEBUG(errs() << MI);
  unsigned int i = 0;
  while (!MI.getOperand(i).isFI()) {
    ++i;
    assert(i < MI.getNumOperands());
  }
  int FI = MI.getOperand(i).getIndex();
  MachineFunction &MF = *MI.getParent()->getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  int64_t offset = MFI.getObjectOffset(FI);
  uint64_t stack_size = MFI.getStackSize() + MFI.getOffsetAdjustment();
  offset += (int64_t)stack_size;
  offset += MI.getOperand(i + 1).getImm();
  MI.getOperand(i).ChangeToRegister(Toy::SP, false);
  MI.getOperand(i + 1).ChangeToImmediate(offset);

  return true;
}

MCPhysReg const *
ToyRegisterInfo::getCalleeSavedRegs(MachineFunction const *MF) const {
  return CSR_SaveList;
}

BitVector ToyRegisterInfo::getReservedRegs(MachineFunction const &MF) const {
  constexpr uint16_t ReservedCPURegs[] = {Toy::ZERO, Toy::RA, Toy::SP};
  BitVector Reserved(getNumRegs());
  for (auto const reg : ReservedCPURegs) {
    Reserved.set(reg);
  }
  return Reserved;
}

Register ToyRegisterInfo::getFrameRegister(MachineFunction const &MF) const {
  return Toy::SP;
}
