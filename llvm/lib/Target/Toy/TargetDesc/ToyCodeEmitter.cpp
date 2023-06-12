// 2023-06-12 16:06
#include "ToyCodeEmitter.h"

using namespace llvm;

void ToyCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                       SmallVectorImpl<MCFixup> &Fixups,
                                       const MCSubtargetInfo &STI) const {}
