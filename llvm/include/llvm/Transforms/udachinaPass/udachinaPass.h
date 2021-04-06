#ifndef LLVM_TRANSFORMS_UDACHINAPASS_H
#define LLVM_TRANSFORMS_UDACHINAPASS_H
#define DEBUG_TYPE "udachinaPass"

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/PassManager.h"

namespace llvm {

	class udachinaPass : public PassInfoMixin<udachinaPass> {
	public:
		PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
		static bool isRequired() { return true; }
	};

}
#endif