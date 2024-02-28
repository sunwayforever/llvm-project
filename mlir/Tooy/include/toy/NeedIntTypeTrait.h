//===- ShapeInferenceInterface.h - Interface definitions for ShapeInference -=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the shape inference interfaces defined
// in ShapeInferenceInterface.td.
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_TUTORIAL_NEED_INT_TYPE_H
#define MLIR_TUTORIAL_NEED_INT_TYPE_H

#include "mlir/IR/OpDefinition.h"

namespace mlir {
namespace OpTrait {
template <typename ConcreteType>
class NeedIntTypeTrait : public TraitBase<ConcreteType, NeedIntTypeTrait> {};
} // namespace opTrait
} // namespace mlir

#endif // MLIR_TUTORIAL_NEED_INT_TYPE_H
