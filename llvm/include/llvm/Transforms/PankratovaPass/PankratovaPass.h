#pragma once

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"

#define DEBUG_TYPE "Pass by A. Pankratova"

STATISTIC(totalDef, "Number of Function definitions");
STATISTIC(totalLoops, "Number of Loops");
STATISTIC(totalBlocks, "Number of Basic blocks");
STATISTIC(totalAdd, "Number of Add instructions");
STATISTIC(totalMul, "Number of Mul instructions");


namespace llvm {

class PankratovaPass : public PassInfoMixin<PankratovaPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  static bool isRequired() { return true; }

};

} // namespace llvm
#undef DEBUG_TYPE
