// 2023-05-22 17:43
#include "ToyTargetObjectFile.h"

using namespace llvm;

void ToyTargetObjectFile::Initialize(MCContext &Ctx, TargetMachine const &TM) {
  TargetLoweringObjectFileELF::Initialize(Ctx, TM);
  InitializeELF(false);
}
