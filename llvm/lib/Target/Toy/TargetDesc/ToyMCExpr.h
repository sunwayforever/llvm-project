// 2023-05-26 11:03
#ifndef TOY_MC_EXPR_H
#define TOY_MC_EXPR_H
#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCValue.h"

namespace llvm {
class ToyMCExpr : public MCTargetExpr {
public:
  enum ToyExprKind {
    TEK_NONE,
    TEK_HI,
    TEK_LO,
  };
  ToyMCExpr(ToyExprKind Kind, const MCExpr *Expr) : Kind(Kind), Expr(Expr) {}
  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;

  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override {
    return false;
  };
  void visitUsedExpr(MCStreamer &Streamer) const override{};
  MCFragment *findAssociatedFragment() const override { return NULL; }
  void fixELFSymbolsInTLSFixups(MCAssembler &Asm) const override{};

private:
  const ToyExprKind Kind;
  const MCExpr *Expr;
};

} // namespace llvm

#endif // TOY_MC_EXPR_H
