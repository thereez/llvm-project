#ifndef LLVM_TRANSFORMS_UTILS_PYANZINPASS1
#define LLVM_TRANSFORMS_UTILS_PYANZINPASS1

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"

namespace llvm {

class PyanzinPass1 : public PassInfoMixin<PyanzinPass1> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired() { return true;}

};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_PYANZINPASS1