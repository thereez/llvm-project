#include "llvm/Transforms/Sazanov_Lab1_Pass/Sazanov_Lab1_Pass.h"

using namespace llvm;

#define DEBUG_TYPE "Sazanov_Lab1_Pass" 
STATISTIC(CountFunc, "Count of function definitions");
STATISTIC(CountLoops, "Count of loops");
STATISTIC(CountBlocks, "Count of blocks");
STATISTIC(CountMul, "Count of mul");
STATISTIC(CountAdd, "Count of add");

void countLoopsRecursively(Loop* loop)
{
	++CountLoops;
	for (auto *subloop : loop->getSubLoops())
	{
		countLoopsRecursively(subloop);
	}
}

PreservedAnalyses Sazanov_Lab1_Pass::run(Function &F, FunctionAnalysisManager &AM)
{
	if (!F.isDeclaration())
	{
		++CountFunc;
	}
	
	LoopInfo &loops = AM.getResult<LoopAnalysis>(F);
	
	for (auto *loop : loops)
	{
		countLoopsRecursively(loop);
	}
	
	for (auto &block : F)
	{
		++CountBlocks;
		for (auto &instruction : block)
		{
			std::string block_name = std::string(instruction.getOpcodeName());
			if ((block_name == "mul") || (block_name == "fmul"))
			{
				++CountMul;
			}
			else if ((block_name == "add") || (block_name == "fadd"))
			{
				++CountAdd;
			}
		}
	}
	
	return PreservedAnalyses::all();
}