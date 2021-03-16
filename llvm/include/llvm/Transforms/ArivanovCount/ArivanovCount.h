#pragma once

#include "llvm/IR/PassManager.h"

namespace llvm {

class ArivanovCountPass : public PassInfoMixin<ArivanovCountPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);

  static bool isRequired() { return true; }

};

}