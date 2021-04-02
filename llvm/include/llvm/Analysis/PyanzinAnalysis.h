#ifndef LLVM_PYANZINANALYSIS_H_
#define LLVM_PYANZINANALYSIS_H_

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/FunctionPropertiesAnalysis.h"

namespace llvm {
class Function;

class PyanzinAnalysisInfo {
public:
  static PyanzinAnalysisInfo getPyanzinAnalysisInfo(const Function &F,
                                                          const LoopInfo &LI);

  void print(raw_ostream &OS) const;
  
  int64_t add_count = 0;
  int64_t mul_count = 0;
  int64_t bb_count = 0;
};

// Analysis pass
class PyanzinAnalysis
    : public AnalysisInfoMixin<PyanzinAnalysis> {

public:
  static AnalysisKey Key;

  using Result = PyanzinAnalysisInfo;

  Result run(Function &F, FunctionAnalysisManager &FAM);
};

/// Printer pass for the FunctionPropertiesAnalysis results.
class PyanzinAnalysisPrinterPass
    : public PassInfoMixin<PyanzinAnalysisPrinterPass> {
  raw_ostream &OS;

public:
  explicit PyanzinAnalysisPrinterPass(raw_ostream &OS) : OS(OS) {}

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm
#endif // LLVM_PYANZINANALYSIS_H_
