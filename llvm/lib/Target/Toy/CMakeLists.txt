add_llvm_component_group(Toy)

set(LLVM_TARGET_DEFINITIONS Toy.td)
tablegen(LLVM ToyGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(ToyCommonTableGen)

add_llvm_target(ToyCodeGen
  ToyTargetMachine.cpp
)

add_subdirectory(TargetInfo)
add_subdirectory(TargetDesc)
