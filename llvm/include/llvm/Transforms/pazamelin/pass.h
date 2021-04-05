#ifndef LLVM_TRANSFORMS_PAZAMELIN_PASS_H
#define LLVM_TRANSFORMS_PAZAMELIN_PASS_H

#include "llvm/Pass.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Bitcode/LLVMBitCodes.h"

namespace llvm
{

class PAZamelinPass : public PassInfoMixin<PAZamelinPass>
{
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  static bool isRequired() { return true; }

  void handleLoop(const Loop* L) const;
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_PAZAMELIN_PASS_H