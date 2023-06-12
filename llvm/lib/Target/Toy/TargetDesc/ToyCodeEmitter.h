// 2023-06-12 15:50
#ifndef TOY_CODE_EMITTER_H
#define TOY_CODE_EMITTER_H
#include "llvm/MC/MCCodeEmitter.h"
using namespace llvm;

namespace llvm {
class MCInstrInfo;
class MCContext;
class MCInst;

class ToyCodeEmitter : public MCCodeEmitter {
public:
    ToyCodeEmitter(const MCInstrInfo &mcii, MCContext &Ctx) {}

  ~ToyCodeEmitter() override {}

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;
};
} // namespace llvm

#endif // TOY_CODE_EMITTER_H
