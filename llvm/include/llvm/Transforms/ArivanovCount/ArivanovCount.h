#pragma once

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#define DEBUG_TYPE "arivanovcount"
STATISTIC(totalFunc, "Number of functions");
STATISTIC(totalLoops, "Number of loops");
STATISTIC(totalBlocks, "Number of Basic blocks");
STATISTIC(totalAdd, "Number of Add instructions");
STATISTIC(totalMul, "Number of Mul instructions");


namespace llvm {

class ArivanovCountPass : public PassInfoMixin<ArivanovCountPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);

  static bool isRequired() { return true; }

};

}