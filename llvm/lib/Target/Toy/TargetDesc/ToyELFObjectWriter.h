// 2023-06-12 16:49
#ifndef TOY_ELF_OBJECT_WRITER_H
#define TOY_ELF_OBJECT_WRITER_H
#include "llvm/MC/MCELFObjectWriter.h"

namespace llvm {
class ToyELFObjectWriter : public MCELFObjectTargetWriter {
public:
  ToyELFObjectWriter(uint8_t OSABI, bool HasRelocationAddend, bool Is64)
      : MCELFObjectTargetWriter(
            /*Is64Bit_=false*/ false, OSABI, ELF::EM_RISCV,
            /*HasRelocationAddend_ = false*/ false) {}

  ~ToyELFObjectWriter() = default;

  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
  bool needsRelocateWithSymbol(const MCSymbol &Sym,
                               unsigned Type) const override {
    return true;
  }
};
} // namespace llvm

#endif // TOY_ELF_OBJECT_WRITER_H
