// 2023-05-23 15:35
#include "ToyInstrInfo.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "ToyGenInstrInfo.inc"

#define GET_REGINFO_HEADER
#include "ToyGenRegisterInfo.inc"

using namespace llvm;

ToyInstrInfo::ToyInstrInfo() : ToyGenInstrInfo() {}

void ToyInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                       MachineBasicBlock::iterator MI,
                                       Register SrcReg, bool isKill, int FI,
                                       const TargetRegisterClass *RC,
                                       const TargetRegisterInfo *TRI,
                                       Register VReg) const {
  DebugLoc DL;
  int Opcode = 0;
  if (Toy::GPRRegClass.hasSubClassEq(RC))
    Opcode = Toy::STOREWFI;
  else if (Toy::FPRRegClass.hasSubClassEq(RC))
    Opcode = Toy::STORFFI;
  else if (Toy::FPR64RegClass.hasSubClassEq(RC))
    Opcode = Toy::STORDFI;
  else {
    llvm_unreachable("Impossible store reg to stack");
  }

  BuildMI(MBB, MI, DL, get(Opcode))
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
  int Opcode = 0;
  if (Toy::GPRRegClass.hasSubClassEq(RC))
    Opcode = Toy::LOADWFI;
  else if (Toy::FPRRegClass.hasSubClassEq(RC))
    Opcode = Toy::LOADFFI;
  else if (Toy::FPR64RegClass.hasSubClassEq(RC))
    Opcode = Toy::LOADDFI;
  else {
    llvm_unreachable("Impossible load reg from stack");
  }

  BuildMI(MBB, MI, DL, get(Opcode), DestReg).addFrameIndex(FI).addImm(0);
}

void ToyInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                               MachineBasicBlock::iterator I,
                               const DebugLoc &DL, MCRegister DestReg,
                               MCRegister SrcReg, bool KillSrc) const {

  int Opcode = 0;
  if (Toy::GPRRegClass.contains(SrcReg)) {
    Opcode = Toy::MV;
  } else if (Toy::FPR64RegClass.contains(SrcReg)) {
    // NOTE: both FPR and FPR64 contains f-regs
    Opcode = Toy::FMVD;
  } else {
    llvm_unreachable("Impossible reg-to-reg copy");
  }

  MachineInstrBuilder MIB = BuildMI(MBB, I, DL, get(Opcode), DestReg);
  MIB.addReg(SrcReg, getKillRegState(KillSrc));
}
