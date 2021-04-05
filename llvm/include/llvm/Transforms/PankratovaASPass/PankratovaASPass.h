#pragma once
#include "llvm/IR/PassManager.h"
#include "llvm/IR/IntrinsicInst.h"

namespace llvm {

class PankratovaASPass : public PassInfoMixin<PankratovaASPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm
