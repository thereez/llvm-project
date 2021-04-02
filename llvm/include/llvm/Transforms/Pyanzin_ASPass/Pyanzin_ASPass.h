#ifndef LLVM_TRANSFORMS_UTILS_PYANZIN_ASPASS
#define LLVM_TRANSFORMS_UTILS_PYANZIN_ASPASS

#include "llvm/IR/PassManager.h"

namespace llvm {

class Pyanzin_ASPass : public PassInfoMixin<Pyanzin_ASPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_PYANZIN_ASPASS