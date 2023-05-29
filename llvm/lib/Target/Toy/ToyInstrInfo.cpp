// 2023-05-23 15:35
#include "ToyInstrInfo.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "ToyGenInstrInfo.inc"

using namespace llvm;

ToyInstrInfo::ToyInstrInfo() : ToyGenInstrInfo() {}

void ToyInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                       MachineBasicBlock::iterator MI,
                                       Register SrcReg, bool isKill, int FI,
                                       const TargetRegisterClass *RC,
                                       const TargetRegisterInfo *TRI,
                                       Register VReg) const {
  DebugLoc DL;
  BuildMI(MBB, MI, DL, get(Toy::STOREFI))
      .addReg(SrcReg, getKillRegState(isKill))
      .addFrameIndex(FI)
      .addImm(0);
}

void ToyInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator MI,
                                        Register DestReg, int FI,
                                        const TargetRegisterClass *RC,
                                        const TargetRegisterInfo *TRI,
                                        Register VReg) const {
  DebugLoc DL;
  BuildMI(MBB, MI, DL, get(Toy::LOADFI), DestReg).addFrameIndex(FI).addImm(0);
}
