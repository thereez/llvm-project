#ifndef LLVM_TRANSFORMS_OKSANAKOZLOVA_OKSANAKOZLOVA_H
#define LLVM_TRANSFORMS_OKSANAKOZLOVA_OKSANAKOZLOVA_H

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"

namespace llvm {

class OksanaKozlovaPass : public PassInfoMixin<OksanaKozlovaPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired() { return true; } 
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_OKSANAKOZLOVA_OKSANAKOZLOVA_H