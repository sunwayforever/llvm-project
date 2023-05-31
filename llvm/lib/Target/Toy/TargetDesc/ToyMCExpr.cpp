// 2023-05-26 11:07
#include "ToyMCExpr.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

void ToyMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  switch (Kind) {
  case TEK_HI:
    OS << "%hi(";
    break;
  case TEK_LO:
    OS << "%lo (";
    break;
  }

  Expr->print(OS, MAI, true);

  switch (Kind) {
  case TEK_HI:
  case TEK_LO:
    OS << ")";
    break;
  }
}
