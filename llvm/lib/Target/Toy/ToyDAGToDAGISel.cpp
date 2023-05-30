// 2023-05-22 14:03
#include "ToyDAGToDAGISel.h"

using namespace llvm;

char ToyDAGToDAGISel::ID = 0;

void ToyDAGToDAGISel::Select(SDNode *N) {
  if (N->isMachineOpcode()) {
    N->setNodeId(-1);
    return;
  }
  SelectCode(N);
}

bool ToyDAGToDAGISel::SelectAddrFI(SDNode *Parent, SDValue Addr, SDValue &Base,
                                   SDValue &Offset) {
  EVT ValTy = Addr.getValueType();
  SDLoc DL(Addr);
  if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), ValTy);
    Offset = CurDAG->getTargetConstant(0, DL, ValTy);
    return true;
  }
  if (CurDAG->isBaseWithConstantOffset(Addr)) {
    ConstantSDNode *CN = dyn_cast<ConstantSDNode>(Addr.getOperand(1));
    FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr.getOperand(0));
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), ValTy);
    Offset = CurDAG->getTargetConstant(CN->getZExtValue(), DL, ValTy);
    return true;
  }
  return false;
}
