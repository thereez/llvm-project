#ifndef FIRSTLAB
#define FIRSTLAB

#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/LoopInfo.h"

namespace llvm {

class BaturinaPass : public PassInfoMixin<BaturinaPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired() { return true; }
};

}

#endif