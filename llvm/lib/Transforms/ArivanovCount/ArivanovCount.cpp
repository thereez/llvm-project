#include "llvm/Transforms/ArivanovCount/ArivanovCount.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

void LoopsCount(Loop* loop){
    totalLoops++;
    auto loops = loop->getSubLoops();
    for (auto loop = loops.begin(); loop != loops.end(); loop++) {
        LoopsCount(*loop);
    }
}

PreservedAnalyses ArivanovCountPass::run(Function &F, FunctionAnalysisManager &FAM) {
  if (!F.isDeclaration()){
	  totalFunc++;
	  llvm::LoopAnalysis::Result& loop = FAM.getResult<LoopAnalysis>(F);
	  for (Loop::iterator iter = loop.begin(); iter != loop.end(); ++iter)
	    LoopsCount(*iter); 
	  for (BasicBlock &bb : F){
		  totalBlocks++;
		  for (Instruction &i : bb){
              std::string instruction = std::string(i.getOpcodeName());
			  if (instruction == "add" || instruction == "fadd") 
				totalAdd++;
              if (instruction == "mul" || instruction == "fmul")
				totalMul++;
		  }
	  }
  }
  return PreservedAnalyses::all();
} 