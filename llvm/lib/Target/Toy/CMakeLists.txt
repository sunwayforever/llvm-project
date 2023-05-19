add_llvm_component_group(Toy)

add_llvm_target(ToyCodeGen
  ToyTargetMachine.cpp
)

add_subdirectory(TargetInfo)
add_subdirectory(TargetDesc)
