// 2023-05-19 11:34
#include "ToyTargetDesc.h"
#include "ToyAsmBackend.h"
#include "ToyInstPrinter.h"
#include "ToyMCCodeEmitter.h"
#include "ToyTargetStreamer.h"
#include "llvm/ADT/None.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "ToyGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "ToyGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "ToyGenRegisterInfo.inc"

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
  InitToyMCRegisterInfo(x, Toy::RA);
  return x;
}

static MCInstPrinter *createToyInstPrinter(Triple const &T,
                                           unsigned SyntaxVariant,
                                           MCAsmInfo const &MAI,
                                           MCInstrInfo const &MII,
                                           MCRegisterInfo const &MRI) {
  return new ToyInstPrinter(MAI, MII, MRI);
}

static MCCodeEmitter *createToyMCCodeEmitter(const MCInstrInfo &MCII,
                                             MCContext &Ctx) {
  return new ToyMCCodeEmitter(MCII, Ctx);
}

static MCAsmBackend *createToyAsmBackend(const Target &T,
                                         const MCSubtargetInfo &STI,
                                         const MCRegisterInfo &MRI,
                                         const MCTargetOptions &Options) {
  return new ToyAsmBackend(T, STI.getTargetTriple());
}

static MCTargetStreamer *createToyTargetStreamer(MCStreamer &S,
                                                 const MCSubtargetInfo &STI) {
  return new ToyTargetStreamer(S, STI);
}

extern "C" void LLVMInitializeToyTargetMC() {
  TargetRegistry::RegisterMCRegInfo(TheToyTarget, createToyMCRegisterInfo);
  TargetRegistry::RegisterMCInstrInfo(TheToyTarget, createToyMCInstrInfo);
  TargetRegistry::RegisterMCSubtargetInfo(TheToyTarget,
                                          createToyMCSubtargetInfo);
  TargetRegistry::RegisterMCAsmInfo(TheToyTarget, createToyMCAsmInfo);
  TargetRegistry::RegisterMCInstPrinter(TheToyTarget, createToyInstPrinter);
  TargetRegistry::RegisterMCCodeEmitter(TheToyTarget, createToyMCCodeEmitter);
  TargetRegistry::RegisterMCAsmBackend(TheToyTarget, createToyAsmBackend);
  TargetRegistry::RegisterObjectTargetStreamer(TheToyTarget,
                                               createToyTargetStreamer);
}
