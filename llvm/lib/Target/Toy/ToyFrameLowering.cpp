// 2023-05-23 11:20
#include "ToyFrameLowering.h"

using namespace llvm;

ToyFrameLowering::ToyFrameLowering()
    : TargetFrameLowering(StackGrowsDown, Align(16), 0) {}

void ToyFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {}

void ToyFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {}

bool ToyFrameLowering::hasFP(const MachineFunction &MF) const { return false; }
