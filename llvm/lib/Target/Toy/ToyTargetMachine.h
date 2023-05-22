// 2023-05-19 15:03
#ifndef TOY_TARGET_MACHINE_H
#define TOY_TARGET_MACHINE_H

#include <llvm/Target/TargetMachine.h>
#include "ToySubtarget.h"

namespace llvm {
class ToyTargetMachine : public LLVMTargetMachine {
private:
  TargetLoweringObjectFile *mTLOF;
  ToySubtarget *mSubtarget;

public:
  ToyTargetMachine(Target const &T, Triple const &TT, StringRef CPU,
                   StringRef FS, TargetOptions const &Options,
                   std::optional<Reloc::Model> RM,
                   std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL,
                   bool JIT);

  ~ToyTargetMachine() {}
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override;
  ToySubtarget const *getSubtargetImpl(Function const &F) const override;
};
} // namespace llvm

#endif // TOY_TARGET_MACHINE_H
