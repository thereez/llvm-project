#include "llvm/Transforms/PyanzinPass1/PyanzinPass1.h"
#define DEBUG_TYPE "pyanzinpass1"

using namespace llvm;

STATISTIC(TotalFuncs, "Number of functions");
STATISTIC(TotalBasicBlocks, "Number of basic blocks");
STATISTIC(TotalLoops, "Number of loops");
STATISTIC(TotalAddInstrs, "Number of add instructions");
STATISTIC(TotalMulInstrs, "Number of mul instructions");

void countLoops(Loop * loop)
{
    TotalLoops++;
    for (auto l = loop->begin(); l != loop->end(); l++)
    countLoops(*l);
}

PreservedAnalyses PyanzinPass1::run(Function &F,
                                      FunctionAnalysisManager &AM) 
                                    {

  TotalFuncs++;
  llvm::LoopAnalysis::Result& loops = AM.getResult<LoopAnalysis>(F);
  for (auto l = loops.begin(); l != loops.end(); l++)
  countLoops(*l);
  for(BasicBlock &BB : F)
  {
      TotalBasicBlocks++;
      for(Instruction &I: BB)
      {
          std::string inst = std::string(I.getOpcodeName());
          if (inst == "add" || inst == "fadd") TotalAddInstrs++;
          if (inst == "mul" || inst == "fmul") TotalMulInstrs++; 
      }
  }


  return PreservedAnalyses::all();
}
