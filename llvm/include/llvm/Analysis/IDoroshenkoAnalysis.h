#ifndef LLVM_ANALYSIS_IDOROSHENKOANALYSIS_H
#define LLVM_ANALYSIS_IDOROSHENKOANALYSIS_H

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
#include <algorithm>
#include <utility>

namespace llvm {

class IDoroshenkoAnalysisInfo {
public:
    void print(raw_ostream &OS) const;
    int64_t TotalPow = 0;
    int64_t TotalAdd = 0;
    int64_t TotalMul = 0;

};

class IDoroshenkoAnalysis : public AnalysisInfoMixin<IDoroshenkoAnalysis> {
public:
    static AnalysisKey Key;
    
    using Result = IDoroshenkoAnalysisInfo;

    Result run(Function& F, FunctionAnalysisManager & FAM);
};

}
#endif