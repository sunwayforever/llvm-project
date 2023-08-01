//===-- PeepholeOptimizationPass.cpp ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PeepholeOptimizationPass.h"
#include "Raiser/MachineInstructionRaiser.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"

using namespace llvm::mctoll;

char PeepholeOptimizationPass::ID = 0;

bool PeepholeOptimizationPass::runOnFunction(Function &F) {
  for (BasicBlock &BB : F) {

    auto It = BB.begin();
    while (It != BB.end()) {
      Instruction &I = *It;
      It++;

      if (auto *I2P = dyn_cast<IntToPtrInst>(&I)) {
        Value *V = I2P->getOperand(0);
        if (auto *BinOp = dyn_cast<BinaryOperator>(V)) {
          /*
            Simplifies the following pattern:
              %tos = ptrtoint i8* %stktop_8 to i64
              %0 = add i64 %tos, 16
              %RBP_N.8 = inttoptr i64 %0 to i32*
            replacing them with:
              %0 = getelementptr i8, i8* %stktop_8, i64 16
              %RBP_N.8 = bitcast i8* %0 to i32*
            This has a significant impact when recompiling the raised program
            with other optimizations, such as -O2.
          */
          auto *P2I = dyn_cast<PtrToIntInst>(BinOp->getOperand(0));
          if (P2I && (BinOp->getOpcode() == Instruction::Add ||
                      BinOp->getOpcode() == Instruction::Or)) {
            auto *Ptr = P2I->getOperand(0);
            auto *PtrElemTy = (isa<IntToPtrInst>(Ptr))
                                  ? dyn_cast<IntToPtrInst>(Ptr)->getSrcTy()
                                  : nullptr;
            if (isa_and_nonnull<IntegerType>(PtrElemTy)) {
              Value *Idx = BinOp->getOperand(1);
              std::vector<Value *> GEPIdx;
              GEPIdx.push_back(Idx);

              IRBuilder<> Builder(&I);

              auto *ElementTy = IntegerType::get(F.getContext(), 8);
              auto *BytePtrTy = PointerType::getUnqual(ElementTy);
              auto *BytePtr = Builder.CreatePointerCast(Ptr, BytePtrTy);
              auto *GEP = Builder.CreateGEP(ElementTy, BytePtr, GEPIdx);

              auto *FinalPtr = Builder.CreatePointerCast(GEP, I2P->getType());
              I2P->replaceAllUsesWith(FinalPtr);

              FinalPtr->takeName(I2P);
              I2P->eraseFromParent();

              if (BinOp->getNumUses() == 0)
                BinOp->eraseFromParent();
              if (P2I->getNumUses() == 0)
                P2I->eraseFromParent();
            }
          }
        } else if (auto *P2I = dyn_cast<PtrToIntInst>(V)) {
          /*
            Simplifies the following pattern into a simple pointer casting.
              %0 = ptrtoint i8* %stktop_8 to i64
              %1 = inttoptr i64 %0 to i32*
          */
          IRBuilder<> Builder(&I);
          auto *FinalPtr =
              Builder.CreatePointerCast(P2I->getOperand(0), I2P->getType());
          I2P->replaceAllUsesWith(FinalPtr);

          FinalPtr->takeName(I2P);
          I2P->eraseFromParent();
          if (P2I->getNumUses() == 0)
            P2I->eraseFromParent();
        }
      }
    }
  }
  return true;
}

void PeepholeOptimizationPass::getAnalysisUsage(AnalysisUsage &AU) const {}
