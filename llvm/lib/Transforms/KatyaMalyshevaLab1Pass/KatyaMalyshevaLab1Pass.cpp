#include "llvm/Transforms/KatyaMalyshevaLab1Pass/KatyaMalyshevaLab1Pass.h"

#define DEBUG_TYPE "KatyaMalyshevaLab1Pass"

using namespace llvm;

STATISTIC(CountDefinitions, "Number of definitions");
STATISTIC(CountLoops, "Number of loops");
STATISTIC(CountBlocks, "Number of basic blocks");
STATISTIC(CountAdd, "Number of add instructions");
STATISTIC(CountMul, "Number of mull instructions");

static void LoopCounter(Loop *L) {
  CountLoops++;
  for (auto loop = L->begin(); loop != L->end(); ++loop) {
    LoopCounter(*loop);
  }
  return;
}

PreservedAnalyses KatyaMalyshevaLab1Pass::run(Function& func, FunctionAnalysisManager& AM) {


	if (!func.isDeclaration()) {
		CountDefinitions++;

	llvm::LoopAnalysis::Result &L = AM.getResult<LoopAnalysis>(func);
                for (auto loop = L.begin(); loop != L.end(); ++loop) {
                  LoopCounter(*loop);
                }

		for (Function::iterator bb = func.begin(); bb != func.end();++bb) {
			CountBlocks++;
			for (BasicBlock::iterator ins = bb->begin(); ins != bb->end(); ++ins) {
				if (ins->getOpcode() == 14) {
				  CountAdd++;
				}
				else if (ins->getOpcode() == 18){
				  CountMul++;
				}

			}
		}
	}
	return PreservedAnalyses::all();
}