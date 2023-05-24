// 2023-05-24 10:16
#ifndef TOY_REGISTER_INFO_H
#define TOY_REGISTER_INFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "ToyGenRegisterInfo.inc"

namespace llvm {
class ToySubtarget;
class ToyRegisterInfo : public ToyGenRegisterInfo {
public:
  ToyRegisterInfo(ToySubtarget const &ST, unsigned HwMode);
  MCPhysReg const *getCalleeSavedRegs(MachineFunction const *MF) const override;
  // uint32_t const *getCallPreservedMask(MachineFunction const
  // &MF,
  //                                      CallingConv::ID) const override;
  BitVector getReservedRegs(MachineFunction const &MF) const override;
  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;
  Register getFrameRegister(MachineFunction const &MF) const override;
};

} // namespace llvm

#endif // TOY_REGISTER_INFO_H
