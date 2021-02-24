//===-- GeexieCount.cpp - Example Transformations --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/GeexieCount/GeexieCount.h"

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/InstIterator.h"

#include <set>

#define DEBUG_TYPE "geexie-count"
#include "llvm/ADT/Statistic.h"

using namespace llvm;

STATISTIC(TotalFuncs, "Number of functions");
STATISTIC(TotalBBs, "Number of basic blocks");
STATISTIC(TotalLoops, "Number of loops");
STATISTIC(TotalMulAndAdds, "Number of multiply and add instructions");
STATISTIC(TotalPhis, "Number of Phi instructions");

void handleLoop(Loop *L) {
  TotalLoops++;
  for (Loop *SL : L->getSubLoops()) {
    handleLoop(SL);
  }
}

PreservedAnalyses GeexieCountPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
  // count functions
  TotalFuncs++;

  // count basic blocks within a function
  for (auto BB = F.begin(); BB != F.end(); ++BB) {
    TotalBBs++;

    // count specific instructions within a basic block
    for (auto I = BB->begin(); I != BB->end(); ++I) {
      std::set<decltype(I->getOpcode())> ioi = {Instruction::Add, Instruction::FAdd, Instruction::Mul, Instruction::FMul};
      if (isa<BinaryOperator>(*I)) {
        if (!!ioi.count(I->getOpcode())) {
          TotalMulAndAdds++;
        }
      }
    }
  }

  // count loops within a function
  auto& LI = AM.getResult<LoopAnalysis>(F);
  for (auto& L : LI) {
    handleLoop(L);
  }

  // count Phi-nodes over all instructions in a block
  for (auto& I : instructions(F)) {
    if (isa<PHINode>(&I)) {
      TotalPhis++;
    }
  }

  return PreservedAnalyses::all();
}
