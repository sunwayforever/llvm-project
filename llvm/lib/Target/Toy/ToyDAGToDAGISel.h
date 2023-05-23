// 2023-05-22 13:59
#ifndef TOY_DAG_TO_DAG_ISEL_H
#define TOY_DAG_TO_DAG_ISEL_H

#include "TargetDesc/ToyTargetDesc.h"
#include "ToyTargetMachine.h"

#include <llvm/CodeGen/SelectionDAGISel.h>

namespace llvm {
class ToyDAGToDAGISel : public SelectionDAGISel {
#include "ToyGenDAGISel.inc"
public:
  static char ID;
  ToyDAGToDAGISel(ToyTargetMachine &TM, CodeGenOpt::Level OL)
      : SelectionDAGISel(ID, TM, OL){};
  StringRef getPassName() const override { return "ToyDAGToDAG"; }
  void Select(SDNode *N) override;

  bool SelectAddrFI(SDNode *Parent, SDValue N, SDValue &Base, SDValue &Offset);
};

} // namespace llvm

#endif // TOY_DAG_TO_DAG_ISEL_H
