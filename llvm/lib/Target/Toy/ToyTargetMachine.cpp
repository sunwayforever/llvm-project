// 2023-05-19 11:06
#include "ToyTargetMachine.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

extern Target TheToyTarget;

extern "C" void LLVMInitializeToyTarget() {
  RegisterTargetMachine<ToyTargetMachine> X(TheToyTarget);
}

ToyTargetMachine::ToyTargetMachine(Target const &T, Triple const &TT,
                                   StringRef CPU, StringRef FS,
                                   TargetOptions const &Options,
                                   std::optional<Reloc::Model> RM,
                                   std::optional<CodeModel::Model> CM,
                                   CodeGenOpt::Level OL, bool JIT)
    // NOTE: 这里的参数主要是 llc 传递过来的命令行参数
    : LLVMTargetMachine(T, "e-m:m-p:32:32-i8:8:32-i16:16:32-i64:64-n32-S64", TT,
                        CPU, FS, Options, Reloc::Static, CodeModel::Small, OL) {
  initAsmInfo();
}
