#include "llvm/Transforms/KovakimyCount/KovakimyCount.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#define DEBUG_TYPE "kovakimycountpass"
STATISTIC(TotalFuncs, "Number of functions");
STATISTIC(TotalLoops, "Number of loops");
STATISTIC(TotalBasicBlocks, "Number of basic blocks");
STATISTIC(TotalArithmInstr, "Number of arithmetic instructions");
STATISTIC(CounterAdd, "Number of adds");
STATISTIC(CounterMult, "Number of muls");

using namespace llvm;

void counterLoop(Loop* loop)
{
    TotalLoops++;
    for (Loop::iterator iter = loop->begin(); iter != loop->end(); ++iter)
        counterLoop(*iter);
}

PreservedAnalyses KovakimyCountPass::run(Function &F, FunctionAnalysisManager &AM) 
{
  if (!F.isDeclaration())
  {
	  TotalFuncs++;
	  llvm::LoopAnalysis::Result& loop = AM.getResult<LoopAnalysis>(F);
	  for (Loop::iterator iter = loop.begin(); iter != loop.end(); ++iter)
	    counterLoop(*iter); 
	  for (Function::iterator iterF = F.begin(); iterF != F.end(); ++iterF)
	  {
		  TotalBasicBlocks++;
		  for (BasicBlock::iterator iterB = iterF->begin(); iterB != iterF->end(); ++iterB)
		  {
			  if (std::string(iterB->getOpcodeName()) == "add" || std::string(iterB->getOpcodeName()) == "fadd") 
				CounterAdd++;
              if (std::string(iterB->getOpcodeName()) == "mul" || std::string(iterB->getOpcodeName()) == "fmul")
				CounterMult++;
		  }
	  }
  }
  return PreservedAnalyses::all();
}