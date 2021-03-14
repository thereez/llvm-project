#ifndef LLVM_TRANSFORMS_MELNIKOVLAB1PASS_MELNIKOVLAB1PASS_H
#define LLVM_TRANSFORMS_MELNIKOVLAB1PASS_MELNIKOVLAB1PASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"


namespace llvm {

class MelnikovLab1Pass : public PassInfoMixin<MelnikovLab1Pass> {
public:
  PreservedAnalyses run(Function &func, FunctionAnalysisManager &AM);

  static bool isRequired() { return true; }  // works even whan all optimisations are turned off 
};

} // namespace llvm

#endif