#ifndef LLVM_TRANSFORMS_UTILS_SOLOVEVAPASS_H
#define LLVM_TRANSFORMS_UTILS_SOLOVEVAPASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Transforms/Utils/LoopUtils.h"

namespace llvm {

class solovevaPass : public PassInfoMixin<solovevaPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
    static bool isRequired() { return true; } 
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_SOLOVEVAPASS2_H