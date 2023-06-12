// 2023-06-12 16:50
#include "ToyELFObjectWriter.h"
using namespace llvm;

unsigned ToyELFObjectWriter::getRelocType(MCContext &Ctx, const MCValue &Target,
                                          const MCFixup &Fixup,
                                          bool IsPCRel) const {
    return ELF::R_RISCV_32;
}
