// 2023-05-22 17:38
#ifndef TOY_TARGET_OBJECT_FILE_H
#define TOY_TARGET_OBJECT_FILE_H

#include <llvm/CodeGen/TargetLoweringObjectFileImpl.h>
#include <llvm/MC/MCContext.h>
#include <llvm/Target/TargetMachine.h>

namespace llvm {

class ToyTargetObjectFile : public TargetLoweringObjectFileELF {
public:
  void Initialize(MCContext &Ctx, TargetMachine const &TM) override;
};
} // namespace llvm

#endif // TOY_TARGET_OBJECT_FILE_H
