// 2023-05-23 11:15
#ifndef TOY_FRAME_LOWERING_H
#define TOY_FRAME_LOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class ToySubtarget;
class ToyFrameLowering : public TargetFrameLowering {
private:
  ToySubtarget &STI;

public:
  ToyFrameLowering(ToySubtarget &sti);
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  bool hasFP(const MachineFunction &MF) const override;
};
} // namespace llvm

#endif // TOY_FRAME_LOWERING_H
