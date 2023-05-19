// 2023-05-19 11:34
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/ADT/None.h"

#define GET_SUBTARGETINFO_MC_DESC
#include "ToyGenSubtargetInfo.inc"

using namespace llvm;

extern Target TheToyTarget;

static MCAsmInfo *createToyMCAsmInfo(MCRegisterInfo const &MRI,
                                     Triple const &TT,
                                     MCTargetOptions const &Options) {
  MCAsmInfo *x = new MCAsmInfo();
  return x;
}

static MCSubtargetInfo *createToyMCSubtargetInfo(Triple const &TT,
                                                 StringRef CPU, StringRef FS) {
  // NOTE: 这是个函数是通过 td 生成的
  return createToyMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}

static MCInstrInfo *createToyMCInstrInfo() {
  MCInstrInfo *x = new MCInstrInfo();
  return x;
}

static MCRegisterInfo *createToyMCRegisterInfo(Triple const &TT) {
  MCRegisterInfo *x = new MCRegisterInfo();
  return x;
}

extern "C" void LLVMInitializeToyTargetMC() {
  TargetRegistry::RegisterMCRegInfo(TheToyTarget, createToyMCRegisterInfo);
  TargetRegistry::RegisterMCInstrInfo(TheToyTarget, createToyMCInstrInfo);
  TargetRegistry::RegisterMCSubtargetInfo(TheToyTarget,
                                          createToyMCSubtargetInfo);
  TargetRegistry::RegisterMCAsmInfo(TheToyTarget, createToyMCAsmInfo);
}
