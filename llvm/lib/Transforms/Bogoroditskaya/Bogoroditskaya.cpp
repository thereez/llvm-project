
#include "llvm/Transforms/Bogoroditskaya/Bogoroditskaya.h"

#define DEBUG_TYPE "Bogoroditskaya"

using namespace llvm;

STATISTIC(TotalFuncs, "Number of functions");
STATISTIC(TotalLoops, "Number of loops");
STATISTIC(TotalBlocks, "Number of Basic blocks");
STATISTIC(TotalAdd, "Number of Arithmetic instructions of type add");
STATISTIC(TotalMull, "Number of Arithmetic instructions of type mull");

void Count_loops(Loop* loops) {
    TotalLoops ++;
    for (Loop::iterator loop  = loops->begin(), loop_end = loops->end(); loop != loop_end; ++loop) 
    {
        Count_loops(*loop);
    }
}

PreservedAnalyses Bogoroditskaya::run(Function &F, FunctionAnalysisManager &AM) 
{
  if (!F.isDeclaration()) 
  {
    TotalFuncs++;                                                                     //подсчитываем количество функций
    llvm::LoopAnalysis::Result& loop_res = AM.getResult<LoopAnalysis>(F);
    for (LoopInfo::iterator i = loop_res.begin(), i_end = loop_res.end(); i != i_end; ++i) 
    { 
      Count_loops(*i); 
    }
                                                                   
    for (Function::iterator i = F.begin(), i_e = F.end(); i != i_e; ++i)              //каждая функция состоит из блоков (i)
    {
      TotalBlocks++;                                                                  //подсчитываем количество базовых блоков
      for ( BasicBlock::iterator j = i->begin(), j_end = i->end(); j != j_end; ++j)       //блоки состоят из инструкций (j)
      {
        std::string temp = j->getOpcodeName();

        if (temp == "fadd" || temp == "add")
        {
          TotalAdd++;
        }
        if (temp == "fmul" || temp == "mul")
        {
          TotalMull++;
        }
      }
    }
  }
  return PreservedAnalyses::all();
}

