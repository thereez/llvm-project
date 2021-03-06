#ifndef LLVM_TRANSFORMS_SAZANOVLAB1PASS_SAZANOVLAB1PASS_H
#define LLVM_TRANSFORMS_SAZANOVLAB1PASS_SAZANOVLAB1PASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Analysis/LoopInfo.h"

namespace llvm
{
	
class Sazanov_Lab1_Pass : public PassInfoMixin<Sazanov_Lab1_Pass>
{

public:
	
	PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
	
	static bool isRequired()
	{
		return true;
	}

};
	
} // namespace llvm

#endif // LLVM_TRANSFORMS_SAZANOVLAB1PASS_SAZANOVLAB1PASS_H