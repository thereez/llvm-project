#include "llvm/Transforms/IlyinPass/IlyinPass.h"

using namespace llvm;

STATISTIC(NumbOfFuncs, "Number of functions");
STATISTIC(NumbOfBasicBlocks, "Number of basic blocks");
STATISTIC(NumbOfLoops, "Number of loops");
STATISTIC(NumbOfAdds, "Number of add instructions");
STATISTIC(NumbOfMulls, "Number of mul instructions");


void LoopsCounter(Loop* l)
{
	++NumbOfLoops;
	for (auto loop = l->begin(); loop != l->end(); loop++) {
		LoopsCounter(*loop);
	}
}


PreservedAnalyses IlyinPass::run(Function& F, FunctionAnalysisManager& AM) {

	if (!F.isDeclaration()) {
		++NumbOfFuncs;
	}
	LoopAnalysis::Result& loops = AM.getResult<LoopAnalysis>(F);
	for (Loop* l : loops) {
		LoopsCounter(l);
	}
	for (BasicBlock& BB : F) {
		++NumbOfBasicBlocks;
		for (Instruction& instr : BB) {
			std::string name = std::string(instr.getOpcodeName());
			if (name == "mul" || name == "fmul") {
				++NumbOfMulls;
			}
			if (name == "add" || name == "fadd") {
				++NumbOfAdds;
			}
		}
	}
	return PreservedAnalyses::all();
}