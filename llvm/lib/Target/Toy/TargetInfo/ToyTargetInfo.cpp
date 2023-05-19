// 2023-05-19 10:43
#include "llvm/IR/Module.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;
Target TheToyTarget;

extern "C" void LLVMInitializeToyTargetInfo() {
  RegisterTarget<Triple::toy, true> X(TheToyTarget, "toy", "Toy RISC-V backend",
                                      "Toy");
}
