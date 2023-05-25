// 2023-05-23 10:20
#ifndef TOY_I_SEL_LOWERING_H
#define TOY_I_SEL_LOWERING_H

#include <llvm/CodeGen/SelectionDAG.h>
#include <llvm/CodeGen/TargetLowering.h>

namespace llvm {

namespace ToyISD {
enum NodeType {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  Hi,
  Lo,
};
}

class ToySubtarget;
class ToyTargetLowering : public TargetLowering {
private:
  SDValue lowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

public:
  ToyTargetLowering(const TargetMachine &TM, ToySubtarget const &STI);

  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               SmallVectorImpl<ISD::InputArg> const &Ins,
                               SDLoc const &dl, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      SmallVectorImpl<ISD::OutputArg> const &Outs,
                      SmallVectorImpl<SDValue> const &OutVals, SDLoc const &dl,
                      SelectionDAG &DAG) const override;
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  const char *getTargetNodeName(unsigned Opcode) const override;
};
} // namespace llvm

#endif // TOY_I_SEL_LOWERING_H
