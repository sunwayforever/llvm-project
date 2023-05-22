// 2023-05-22 14:03
#include "ToyDAGToDAGISel.h"

using namespace llvm;

char ToyDAGToDAGISel::ID = 0;

void ToyDAGToDAGISel::Select(SDNode *N) { SelectCode(N); }
