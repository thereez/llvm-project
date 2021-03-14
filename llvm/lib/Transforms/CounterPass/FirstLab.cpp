#include "llvm/Transforms/CounterPass/FirstLab.h"


using namespace llvm;

STATISTIC(CountDefinitions, "Number of functions definitions");
STATISTIC(CountLoops, "Number of loops");
STATISTIC(CountBasicBlocks, "Number of BasicBlocks");
STATISTIC(CountAdds, "Number of Adds");
STATISTIC(CountMulls, "Number of Mulls");

void CountLoop(Loop* loop)
{
	CountLoops++;
	for (Loop::iterator i = loop->begin(); i != loop->end(); ++i)
		CountLoop(*i);
}



PreservedAnalyses CounterPass::run(Function &F, FunctionAnalysisManager &AM) {
  if (!F.isDeclaration())
    CountDefinitions++;

  llvm::LoopAnalysis::Result& loop = AM.getResult<LoopAnalysis>(F);
  for (Loop::iterator i = loop.begin(); i != loop.end(); ++i)
	CountLoop(*i);

  for (Function::iterator i = F.begin(); i != F.end(); ++i) {
    CountBasicBlocks++;
    for (BasicBlock::iterator j = i->begin();j != i->end(); ++j) 
	{
      if ((std::string(j->getOpcodeName()) == "mul") || (std::string(j->getOpcodeName()) == "fmul"))
        CountMulls++;
      if ((std::string(j->getOpcodeName()) == "add") || (std::string(j->getOpcodeName()) == "fadd"))
        CountAdds++;
      
    }
  }

  return PreservedAnalyses::all();
}