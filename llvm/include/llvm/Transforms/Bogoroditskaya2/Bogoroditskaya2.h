#ifndef LLVM_TRANSFORMS_UTILS_BOGORODITSKAYA2_H
#define LLVM_TRANSFORMS_UTILS_BOGORODITSKAYA2_H

#include "llvm/IR/PassManager.h"

namespace llvm
{

class LMPUpdater;

class Bogoroditskaya2 : public PassInfoMixin<Bogoroditskaya2> 
{
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} 

#endif // LLVM_TRANSFORMS_UTILS_BOGORODITSKAYA2_H
