// 2023-05-22 18:10
#ifndef TOY_SUBTARGET_H
#define TOY_SUBTARGET_H

#include <llvm/ADT/None.h>
#include <llvm/CodeGen/TargetSubtargetInfo.h>
#include <llvm/MC/MCInst.h>

#define GET_SUBTARGETINFO_HEADER
#include "ToyGenSubtargetInfo.inc"

namespace llvm {
class ToyTargetMachine;
class ToySubtarget : public ToyGenSubtargetInfo {
public:
  ToySubtarget(Triple const &TT, StringRef &CPU, StringRef &TuneCPU,
               StringRef &FS, ToyTargetMachine const &TTM);

  ~ToySubtarget() override {}

  void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);
};
} // namespace llvm

#endif // TOY_SUBTARGET_H
