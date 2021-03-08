#pragma once

#ifndef LLVM_TRANSFORMS_ILYINPASS_ILYINPASS_H
#define LLVM_TRANSFORMS_ILYINPASS_ILYINPASS_H

#define DEBUG_TYPE "IlyinPass"

#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"

namespace llvm {

	class IlyinPass : public PassInfoMixin<IlyinPass> {
	public:

		PreservedAnalyses run(Function& F, FunctionAnalysisManager& AM);

		static bool isRequired() { return true; }  

	};

}


#endif 

