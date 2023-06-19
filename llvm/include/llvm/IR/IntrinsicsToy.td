let TargetPrefix = "toy" in {
  def int_toy_getsp :
      ClangBuiltin<"__builtin_getsp">,
      Intrinsic<[llvm_ptr_ty], [], [IntrNoMem]>;
}
