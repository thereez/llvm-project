#ifndef LLVM_TRANSFORMS_UTILS_PYANZINPASS3
#define LLVM_TRANSFORMS_UTILS_PYANZINPASS3

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"

#include "llvm/Analysis/FunctionPropertiesAnalysis.h"
#include "llvm/Analysis/PyanzinAnalysis.h"
#include "llvm/Analysis/LoopAnalysisManager.h"

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PyanzinLoopAnalysis.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"

namespace llvm {

class PyanzinPass3 : public PassInfoMixin<PyanzinPass3> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired() { return true;}

};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_PYANZINPASS3