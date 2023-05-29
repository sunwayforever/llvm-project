add_llvm_component_group(Toy)

set(LLVM_TARGET_DEFINITIONS Toy.td)
tablegen(LLVM ToyGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM ToyGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM ToyGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM ToyGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM ToyGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM ToyGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM ToyGenCallingConv.inc -gen-callingconv)
add_public_tablegen_target(ToyCommonTableGen)

add_llvm_target(ToyCodeGen
  ToyTargetMachine.cpp
  ToyDAGToDAGISel.cpp
  ToyAsmPrinter.cpp
  ToyTargetObjectFile.cpp
  ToySubtarget.cpp
  ToyISelLowering.cpp
  ToyFrameLowering.cpp
  ToyInstrInfo.cpp
  ToyRegisterInfo.cpp
  ToyMCInstLower.cpp
)

add_subdirectory(TargetInfo)
add_subdirectory(TargetDesc)
