
#ifndef LLVM_TRANSFORMS_UTILS_BOGORODITSKAYA_H
#define LLVM_TRANSFORMS_UTILS_BOGORODITSKAYA_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Transforms/Utils/LoopUtils.h"

namespace llvm
{

class Bogoroditskaya : public PassInfoMixin<Bogoroditskaya> 
{
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired() { return true; } 
};

} 

#endif // LLVM_TRANSFORMS_UTILS_BOGORODITSKAYA_H
