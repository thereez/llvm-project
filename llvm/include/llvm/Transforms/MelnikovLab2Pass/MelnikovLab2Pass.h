#ifndef LLVM_TRANSFORMS_MELNIKOVLAB2PASS_MELNIKOVLAB2PASS_H
#define LLVM_TRANSFORMS_MELNIKOVLAB2PASS_MELNIKOVLAB2PASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"


namespace llvm {

class MelnikovLab2Pass : public PassInfoMixin<MelnikovLab2Pass> {
public:
  PreservedAnalyses run(Function &func, FunctionAnalysisManager &AM);

  static bool isRequired() { return true; }  
};

} 

#endif