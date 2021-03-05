#include "llvm/Transforms/Sazanov_Lab1_Pass.h"

using namespace llvm;

STATISTIC(CountFunc, "Count of function definitions");
STATISTIC(CountLoops, "Count of loops");
STATISTIC(CountBlocks, "Count of blocks");
STATISTIC(CountMul, "Count of mul");
STATISTIC(CountAdd, "Count of add");

void countLoopsRecursively(Loop* loop)
{
	++CountLoops;
	for (auto *subloop : loop->getSubloops())
	{
		countLoopsRecursively(subloop);
	}
}

PreservedAnalyses run(Function &F, FunctionAlanysisManager &AM)
{
	if (!F.isDeclaration())
	{
		++CountFunc;
	}
	
	LoopInfo &loops = AM.getResult<LoopAnalysis>(func);
	
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
			if ((name == "mul") || (name == "fmul"))
			{
				++CountMul;
			}
			else if ((name == "add") || (name == "fadd"))
			{
				++CountAdd;
			}
		}
	}
	
	return PreservedAnalyses::all();
}