#pragma once
#ifndef LLVM_TRANSFORMS_KATYAMALYSHEVALAB1PASS_KATYAMALYSHEVALAB1PASS_H
#define LLVM_TRANSFORMS_KATYAMALYSHEVALAB1PASS_KATYAMALYSHEVALAB1PASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"


namespace llvm {

	class KatyaMalyshevaLab1Pass : public PassInfoMixin<KatyaMalyshevaLab1Pass> {
	public:
		PreservedAnalyses run(Function& func, FunctionAnalysisManager& AM);

		static bool isRequired() { return true; }
	};

}

#endif 