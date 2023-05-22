// 2023-05-22 17:01
#include "ToyAsmPrinter.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

extern Target TheToyTarget;

extern "C" void LLVMInitializeToyAsmPrinter() {
  RegisterAsmPrinter<ToyAsmPrinter> X(TheToyTarget);
}
