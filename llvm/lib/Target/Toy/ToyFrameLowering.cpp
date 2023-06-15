// 2023-05-23 11:20
#include "ToyFrameLowering.h"
#include "TargetDesc/ToyTargetDesc.h"
#include "ToySubtarget.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include <llvm/CodeGen/MachineFrameInfo.h>
#include <llvm/CodeGen/MachineFunction.h>
#include <llvm/MC/MCInstrInfo.h>
using namespace llvm;

ToyFrameLowering::ToyFrameLowering(ToySubtarget &sti)
    : TargetFrameLowering(StackGrowsDown, Align(16), 0, Align(16)), STI(sti) {}

void ToyFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.begin();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const ToyInstrInfo &TII =
      *static_cast<const ToyInstrInfo *>(STI.getInstrInfo());
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  uint64_t StackSize = MFI.getStackSize() + MFI.getOffsetAdjustment();

  if (StackSize == 0 && !MFI.adjustsStack())
    return;
  BuildMI(MBB, MBBI, DL, TII.get(Toy::ADDI), Toy::SP)
      .addReg(Toy::SP)
      .addImm(-StackSize);
}

void ToyFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getFirstTerminator();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const ToyInstrInfo &TII =
      *static_cast<const ToyInstrInfo *>(STI.getInstrInfo());
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  uint64_t StackSize = MFI.getStackSize() + MFI.getOffsetAdjustment();

  if (StackSize == 0 && !MFI.adjustsStack())
    return;
  BuildMI(MBB, MBBI, DL, TII.get(Toy::ADDI), Toy::SP)
      .addReg(Toy::SP)
      .addImm(StackSize);
}

bool ToyFrameLowering::hasFP(const MachineFunction &MF) const { return false; }

void ToyFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                            BitVector &SavedRegs,
                                            RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  if (MF.getFrameInfo().hasCalls()) {
    SavedRegs.set(Toy::RA);
  }
}
