// 2023-06-12 16:37
#include "ToyAsmBackend.h"
#include "ToyELFObjectWriter.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixupKindInfo.h"

using namespace llvm;

const MCFixupKindInfo &ToyAsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  const static MCFixupKindInfo Infos[Toy::NumTargetFixupKinds] = {
      // name                      offset bits  flags
      {"fixup_riscv_hi20", 12, 20, 0},
      {"fixup_riscv_lo12_i", 20, 12, 0},
  };

  if (Kind < FirstTargetFixupKind)
    return MCAsmBackend::getFixupKindInfo(Kind);

  assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
         "Invalid kind!");
  return Infos[Kind - FirstTargetFixupKind];
}

static std::unique_ptr<MCObjectTargetWriter>
createToyELFObjectWriter(const Triple &TT) {
  uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TT.getOS());
  bool IsN64 = false;
  bool HasRelocationAddend = false;
  return std::make_unique<ToyELFObjectWriter>(OSABI, HasRelocationAddend,
                                              IsN64);
}

std::unique_ptr<MCObjectTargetWriter>
ToyAsmBackend::createObjectTargetWriter() const {
  return createToyELFObjectWriter(TheTriple);
}
