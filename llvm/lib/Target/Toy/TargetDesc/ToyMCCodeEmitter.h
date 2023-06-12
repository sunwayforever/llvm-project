// 2023-06-12 15:50
#ifndef TOY_CODE_EMITTER_H
#define TOY_CODE_EMITTER_H
#include "ToyTargetDesc.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/Error.h"
#include <stdint.h>
using namespace llvm;

namespace llvm {
class MCInstrInfo;
class MCContext;
class MCInst;

class ToyMCCodeEmitter : public MCCodeEmitter {
  MCContext &Ctx;

public:
  ToyMCCodeEmitter(const MCInstrInfo &mcii, MCContext &Ctx) : Ctx(Ctx) {}

  ~ToyMCCodeEmitter() override {}

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;
  // NOTE: auto gen in "ToyGenMCCodeEmitter.inc"
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;
  // NOTE: called by ToyGenMCCodeEmitter
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

};
} // namespace llvm

#endif // TOY_CODE_EMITTER_H
