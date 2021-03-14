#ifndef LLVM_TRANSFORMS_AKOMYAGINASPASS_AKOMYAGINASPASS_H
#define LLVM_TRANSFORMS_AKOMYAGINASPASS_AKOMYAGINASPASS_H

#define DEBUG_TYPE "AKomyaginASPass"

#include "llvm/IR/PassManager.h"


namespace llvm {

  class AKomyaginASPass : public PassInfoMixin<AKomyaginASPass> {
  public:
    PreservedAnalyses run(Function& F, FunctionAnalysisManager& AM);
  };
}
#undef DEBUG_TYPE
#endif