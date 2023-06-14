// 2023-05-26 11:03
#ifndef TOY_MC_EXPR_H
#define TOY_MC_EXPR_H
#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCValue.h"

namespace llvm {
class ToyMCExpr : public MCTargetExpr {
public:
  enum ToyExprKind {
    TEK_NONE,
    TEK_HI,
    TEK_LO,
    TEK_JAL,
    TEK_BRANCH,
  };
  ToyMCExpr(ToyExprKind Kind, const MCExpr *Expr) : Kind(Kind), Expr(Expr) {}
  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;

  void visitUsedExpr(MCStreamer &Streamer) const override {
    // NOTE: ToyMCExpr 的 kind 是 Target, 导致 MCStreamer 不知道如何 visit 它的
    // operand, 需要自己实现. visitUsedExpr 会把 operand 中用到的 symbol ref 加
    // 入到符号表中
    Streamer.visitUsedExpr(*Expr);
  };

  MCFragment *findAssociatedFragment() const override { return NULL; }
  void fixELFSymbolsInTLSFixups(MCAssembler &Asm) const override{};
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override;
  const MCExpr *getSubExpr() const { return Expr; }
  ToyExprKind getKind() const { return Kind; }

private:
  const ToyExprKind Kind;
  const MCExpr *Expr;
};

} // namespace llvm

#endif // TOY_MC_EXPR_H
