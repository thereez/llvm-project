#ifndef LLVM_TRANSFORMS_PAZAMELIN_AS_PASS_H
#define LLVM_TRANSFORMS_PAZAMELIN_AS_PASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/SmallVector.h"

namespace llvm
{

class PAZamelinPassAS : public PassInfoMixin<PAZamelinPassAS>
{
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired() { return true; }
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_PAZAMELIN_AS_PASS_H