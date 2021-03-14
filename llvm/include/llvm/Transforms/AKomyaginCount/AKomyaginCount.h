#ifndef LLVM_TRANSFORMS_AKOMYAGINCOUNT_AKOMYAGINCOUNT_H
#define LLVM_TRANSFORMS_AKOMYAGINCOUNT_AKOMYAGINCOUNT_H

#define DEBUG_TYPE "AKomyaginCount"

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Analysis/LoopInfo.h"

STATISTIC(FunctionCount, "Number of function definitions");
STATISTIC(LoopsCount, "Number of loops");
STATISTIC(BlocksCount, "Number of basic blocks");
STATISTIC(MulCount, "Number of mul");
STATISTIC(AddCount, "Number of add");

namespace llvm {

  class AKomyaginCount : public PassInfoMixin<AKomyaginCount> {
  public:
    PreservedAnalyses run(Function& F, FunctionAnalysisManager& AM);
    static bool isRequired() { return true; }
  };
}

#endif // LLVM_TRANSFORMS_UTILS_HELLOWORLD_H