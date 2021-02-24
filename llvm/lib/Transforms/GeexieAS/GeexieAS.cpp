//===-- GeexieAS.cpp - Example Transformations --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/GeexieAS/GeexieAS.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"

#define DEBUG_TYPE "geexie-as"
#include "llvm/ADT/Statistic.h"

using namespace llvm;

// Example pass which makes algebraic simplification y = x + 0.f -> x
PreservedAnalyses GeexieASPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
  auto pa = PreservedAnalyses::all();

  // iterate over instructions to find candidates
  SmallVector<BinaryOperator*, 16> wark_set;
  for (auto& I : instructions(F)) {
    if (auto BO = dyn_cast<BinaryOperator>(&I)) {
      if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1))) {
        if (RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) {
          wark_set.push_back(BO);
          pa = PreservedAnalyses::none();
        }
      }
    }
  }

  // remove redundant operations
  while (!wark_set.empty()) {
    auto BO = wark_set.pop_back_val();
    BO->replaceAllUsesWith(BO->getOperand(0));
    BO->eraseFromParent();
  }

  // preserve analysis iff none of instruction is removed
  return pa;
}
