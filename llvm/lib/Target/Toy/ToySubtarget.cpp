// 2023-05-22 18:36
#include "ToySubtarget.h"

#define DEBUG_TYPE "toy subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "ToyGenSubtargetInfo.inc"

using namespace llvm;

ToySubtarget::ToySubtarget(Triple const &TT, StringRef &CPU, StringRef &TuneCPU,
                           StringRef &FS, TargetMachine &TM)
    : ToyGenSubtargetInfo(TT, CPU, TuneCPU, FS), mTargetLowering(TM, *this),
      mRegisterInfo(*this, getHwMode()) {}

ToyTargetLowering const *ToySubtarget::getTargetLowering() const {
  return &mTargetLowering;
}

ToyFrameLowering const *ToySubtarget::getFrameLowering() const {
  return &mFrameLowering;
}

ToyInstrInfo const *ToySubtarget::getInstrInfo() const { return &mInstrInfo; }

ToyRegisterInfo const *ToySubtarget::getRegisterInfo() const {
  return &mRegisterInfo;
}
