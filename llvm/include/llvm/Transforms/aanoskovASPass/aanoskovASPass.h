#ifndef LLVM_TRANSFORMS_AANOSKOVASPASS_H
#define LLVM_TRANSFORMS_AANOSKOVASPASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Instructions.h"
/*#include "llvm/IR/Module.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/ADT/Statistic.h"*/

namespace llvm {

class LPMUpdater;

class aanoskovASPass : public PassInfoMixin<aanoskovASPass> {
    public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};


} // namespace llvm

#endif // LLVM_TRANSFORMS_AANOSKOVASPASS_H
