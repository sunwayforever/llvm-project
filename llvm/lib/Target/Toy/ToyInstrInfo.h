// 2023-05-23 15:34
#ifndef TOY_INSTR_INFO_H
#define TOY_INSTR_INFO_H

#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "ToyGenInstrInfo.inc"

namespace llvm {
class ToyInstrInfo : public ToyGenInstrInfo {

public:
  ToyInstrInfo();
};

} // namespace llvm

#endif // TOY_INSTR_INFO_H
