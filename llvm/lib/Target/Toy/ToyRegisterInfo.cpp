// 2023-05-24 10:18
#include "ToyRegisterInfo.h"
#include "TargetDesc/ToyTargetDesc.h"
#include "ToySubtarget.h"

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
  return true;
}

MCPhysReg const *
ToyRegisterInfo::getCalleeSavedRegs(MachineFunction const *MF) const {
  return CSR_SaveList;
}

BitVector ToyRegisterInfo::getReservedRegs(MachineFunction const &MF) const {
  constexpr uint16_t ReservedCPURegs[] = {Toy::ZERO};
  BitVector Reserved(getNumRegs());
  for (auto const reg : ReservedCPURegs) {
    Reserved.set(reg);
  }
  return Reserved;
}

Register ToyRegisterInfo::getFrameRegister(MachineFunction const &MF) const {
  return Toy::SP;
}
