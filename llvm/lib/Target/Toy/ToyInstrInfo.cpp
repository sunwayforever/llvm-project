// 2023-05-23 15:35
#include "ToyInstrInfo.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "ToyGenInstrInfo.inc"

using namespace llvm;

ToyInstrInfo::ToyInstrInfo() : ToyGenInstrInfo() {}
