#pragma once

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"

namespace llvm {

	class Strakhovcounter : public PassInfoMixin<Strakhovcounter> {
	public:
		PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
		static bool isRequired() { return true; }
	};

}
