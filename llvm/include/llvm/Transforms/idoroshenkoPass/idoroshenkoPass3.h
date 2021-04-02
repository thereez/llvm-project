
#ifndef LLVM_TRANSFORMS_UTILS_IDOROSHENKOPASS3_H
#define LLVM_TRANSFORMS_UTILS_IDOROSHENKOPASS3_H

#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Transforms/Utils/LoopUtils.h"

namespace llvm {

class IDoroshenkoPass3 : public PassInfoMixin<IDoroshenkoPass3> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
    static bool isRequired() { return true; } 
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_IDOROSHENKOPASS3_H
