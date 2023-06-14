// 2023-06-14 14:00
#include "ToyTargetStreamer.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCELFStreamer.h"

using namespace llvm;

void ToyTargetStreamer::finish() {
  {
    MCELFStreamer &streamer = static_cast<MCELFStreamer &>(Streamer);
    MCAssembler &MCA = streamer.getAssembler();
    unsigned EFlags = MCA.getELFHeaderEFlags();
    EFlags |= ELF::EF_RISCV_FLOAT_ABI_DOUBLE;
    MCA.setELFHeaderEFlags(EFlags);
  }
}
