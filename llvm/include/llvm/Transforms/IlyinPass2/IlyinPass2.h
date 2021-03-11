#pragma once

#ifndef LLVM_TRANSFORMS_ILYINPASS2_ILYINPASS2_H
#define LLVM_TRANSFORMS_ILYINPASS2_ILYINPASS2_H

#define DEBUG_TYPE "IlyinPass2"

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"

namespace llvm {

	class IlyinPass2 : public PassInfoMixin<IlyinPass2> {
	public:

		PreservedAnalyses run(Function& F, FunctionAnalysisManager& AM);

		static bool isRequired() { return true; }  

	};

}

#endif 

