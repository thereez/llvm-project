#pragma once
#include "llvm/IR/PassManager.h"
#include "llvm/IR/IntrinsicInst.h"

#define DEBUG_TYPE "pankratova-as"

namespace llvm {

class PankratovaASPass : public PassInfoMixin<PankratovaASPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm
