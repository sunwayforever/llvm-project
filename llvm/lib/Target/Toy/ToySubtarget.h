// 2023-05-22 18:10
#ifndef TOY_SUBTARGET_H
#define TOY_SUBTARGET_H

#include "ToyISelLowering.h"
#include "ToyFrameLowering.h"
#include <llvm/ADT/None.h>
#include <llvm/CodeGen/TargetSubtargetInfo.h>
#include <llvm/MC/MCInst.h>

#define GET_SUBTARGETINFO_HEADER
#include "ToyGenSubtargetInfo.inc"

namespace llvm {
class ToyTargetMachine;
class ToySubtarget : public ToyGenSubtargetInfo {
private:
  ToyTargetLowering mTargetLowering;
  ToyFrameLowering mFrameLowering;

public:
  ToySubtarget(Triple const &TT, StringRef &CPU, StringRef &TuneCPU,
               StringRef &FS, TargetMachine &TTM);

  ~ToySubtarget() override {}

  ToyTargetLowering const *getTargetLowering() const override;
  ToyFrameLowering const *getFrameLowering() const override;

  void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);
};
} // namespace llvm

#endif // TOY_SUBTARGET_H
