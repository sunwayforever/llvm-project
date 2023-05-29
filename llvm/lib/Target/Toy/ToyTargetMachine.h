// 2023-05-19 15:03
#ifndef TOY_TARGET_MACHINE_H
#define TOY_TARGET_MACHINE_H

#include "llvm/Target/TargetMachine.h"

namespace llvm {
class ToyTargetMachine : public LLVMTargetMachine {
public:
  ToyTargetMachine(Target const &T, Triple const &TT, StringRef CPU,
                   StringRef FS, TargetOptions const &Options,
                   std::optional<Reloc::Model> RM,
                   std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL,
                   bool JIT);

  ~ToyTargetMachine() {}
};
} // namespace llvm

#endif // TOY_TARGET_MACHINE_H
