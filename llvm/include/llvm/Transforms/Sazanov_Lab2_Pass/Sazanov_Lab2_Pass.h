#ifndef LLVM_TRANSFORMS_SAZANOVLAB1PASS_SAZANOVLAB2PASS_H
#define LLVM_TRANSFORMS_SAZANOVLAB1PASS_SAZANOVLAB2PASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

namespace llvm 
{
	
class Sazanov_Lab2_Pass : public PassInfoMixin<Sazanov_Lab2_Pass>
{
	
public:

	PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

	static bool isRequired()
	{
		return true;
	}

};
		
} //namespace llvm

#endif //LLVM_TRANSFORMS_SAZANOVLAB1PASS_SAZANOVLAB2PASS_H