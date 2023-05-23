// 2023-05-23 10:23
#include "ToyISelLowering.h"

using namespace llvm;

ToyTargetLowering::ToyTargetLowering(const TargetMachine &TM,
                                     ToySubtarget const &STI)
    : TargetLowering(TM) {}

SDValue ToyTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    SmallVectorImpl<ISD::InputArg> const &Ins, SDLoc const &dl,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  return Chain;
}

SDValue
ToyTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               SmallVectorImpl<ISD::OutputArg> const &Outs,
                               SmallVectorImpl<SDValue> const &OutVals,
                               SDLoc const &dl, SelectionDAG &DAG) const {
  return Chain;
}
