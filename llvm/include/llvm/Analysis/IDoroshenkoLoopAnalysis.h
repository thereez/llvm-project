#ifndef LLVM_ANALYSIS_IDOROSHENKOLOOPANALYSIS_H
#define LLVM_ANALYSIS_IDOROSHENKOLOOPANALYSIS_H

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/GraphTraits.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include <algorithm>
#include <utility>

namespace llvm {

class IDoroshenkoLoopAnalysisInfo {
public:

    int64_t InvUpdatesCount = -1;
    
};

class IDoroshenkoLoopAnalysis : public AnalysisInfoMixin<IDoroshenkoLoopAnalysis> {
public:
    static AnalysisKey Key;
    
    using Result = IDoroshenkoLoopAnalysisInfo;

    Result run(Loop& L, LoopAnalysisManager& LAM, LoopStandardAnalysisResults& LSA);

    static StringRef name() { return "IDoroshenkoLoopAnalysis"; }
};

}
#endif