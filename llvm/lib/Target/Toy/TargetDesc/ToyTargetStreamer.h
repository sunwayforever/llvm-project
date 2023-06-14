// 2023-06-14 13:56
#ifndef TOY_TARGET_STREAM_H
#define TOY_TARGET_STREAM_H

#include "llvm/MC/MCStreamer.h"

namespace llvm {
class ToyTargetStreamer : public MCTargetStreamer {
public:
  ToyTargetStreamer(MCStreamer &S, const MCSubtargetInfo &STI)
      : MCTargetStreamer(S) {}

  void finish() override;
};

} // namespace llvm

#endif // TOY_TARGET_STREAM_H
