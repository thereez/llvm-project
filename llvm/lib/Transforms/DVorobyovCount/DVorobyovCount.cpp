#include "llvm/Transforms/DVorobyovCount/DVorobyovCount.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#define DEBUG_TYPE "dvorobyovcountpass"
STATISTIC(TotalFuncs, "Number of functions");
STATISTIC(TotalLoops, "Number of loops");
STATISTIC(TotalBasicBlocks, "Number of basic blocks");
STATISTIC(CountingAdds, "Number of adds");
STATISTIC(CountingMults, "Number of muls");

using namespace llvm;

void counterLoop(Loop* loop)
{
    TotalLoops++;
    for (auto id = loop->begin(); id != loop->end(); ++id)
        counterLoop(*id);
}

PreservedAnalyses DVorobyovCountPass::run(Function &F, FunctionAnalysisManager &AM) 
{
  if (F.isDeclaration())
  {
	  return 1;
  }
  else{
	  TotalFuncs++;
	  llvm::LoopAnalysis::Result& result = AM.getResult<LoopAnalysis>(F);
	  for (auto id = result.begin(); id != result.end(); ++id)
	    counterLoop(*id); 
	  for (auto idF = F.begin(); idF != F.end(); ++idF)
	  {
		  TotalBasicBlocks++;
		  for (auto idB = idF->begin(); idB != idF->end(); ++idB)
		  {
			  std::string str = std::string(idB->getOpcodeName());
			  if (str == "add" || str == "fadd") 
				CountingAdds++;
              if (str == "mul" || str == "fmul")
				CountingMults++;
		  }
	  }
  }
  return PreservedAnalyses::all();
} 