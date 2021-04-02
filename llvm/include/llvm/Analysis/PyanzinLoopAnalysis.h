#ifndef LLVM_PYANZINLOOPANALYSIS_H_
#define LLVM_PYANZINLOOPANALYSIS_H_

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/ADT/SmallVector.h"


namespace llvm {
class PyanzinLoopAnalysis : public AnalysisInfoMixin<PyanzinLoopAnalysis> {
  

public:
    static AnalysisKey Key;
    struct Result {
    int64_t updates_count = 0;
  };
  Result run(Loop& L, LoopAnalysisManager& LAM, LoopStandardAnalysisResults& LSA);
  static StringRef name() { return "PyanzinLoopAnalysis"; }
};
} // namespace llvm

#endif // LLVM_PYANZINLOOPANALYSIS_H_