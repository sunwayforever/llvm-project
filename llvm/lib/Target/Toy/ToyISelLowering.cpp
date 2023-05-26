// 2023-05-23 10:23
#include "ToyISelLowering.h"
#include "TargetDesc/ToyBaseInfo.h"
#include "TargetDesc/ToyTargetDesc.h"

#define DEBUG_TYPE "toy isel lowering"

using namespace llvm;

ToyTargetLowering::ToyTargetLowering(const TargetMachine &TM,
                                     ToySubtarget const &STI)
    : TargetLowering(TM) {
  setOperationAction(ISD::GlobalAddress, MVT::i32, Custom);
}

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
                               SDLoc const &DL, SelectionDAG &DAG) const {
  SmallVector<SDValue, 4> Ops(1, Chain);
  Ops[0] = Chain;
  return DAG.getNode(ToyISD::Ret, DL, MVT::Other, Ops);
}

SDValue ToyTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  case ISD::GlobalAddress:
    return lowerGlobalAddress(Op, DAG);
  }
  return SDValue();
}

SDValue ToyTargetLowering::lowerGlobalAddress(SDValue Op,
                                              SelectionDAG &DAG) const {
  EVT Ty = Op.getValueType();
  GlobalAddressSDNode *N = cast<GlobalAddressSDNode>(Op);
  SDLoc DL(N);
  SDValue Hi =
      DAG.getTargetGlobalAddress(N->getGlobal(), DL, Ty, 0, ToyII::MO_HI);
  SDValue Lo =
      DAG.getTargetGlobalAddress(N->getGlobal(), DL, Ty, 0, ToyII::MO_LO);
  // return DAG.getNode(ISD::ADD, DL, Ty, DAG.getNode(ToyISD::Hi, DL, Ty, Hi),
  //                    DAG.getNode(ToyISD::Lo, DL, Ty, Lo));
  SDValue MNHi = SDValue(DAG.getMachineNode(Toy::LUI, DL, Ty, Hi), 0);
  return SDValue(DAG.getMachineNode(Toy::ADDI, DL, Ty, MNHi, Lo), 0);
}

const char *ToyTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case ToyISD::Hi:
    return "ToyISD::Hi";
  case ToyISD::Lo:
    return "ToyISD::Lo";
  case ToyISD::Ret:
    return "ToyISD::Ret";
  default:
    return NULL;
  }
}
