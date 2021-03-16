#include "llvm/Transforms/VokhmyaninaCounter/VokhmyaninaCounter.h"

using namespace llvm;

STATISTIC(TotalFuncsDef, "Number of function definitions");
STATISTIC(TotalLoops, "Number of loops");
STATISTIC(TotalBasicBlocks, "Number of basic blocks");
STATISTIC(TotalAdd, "Number of add");
STATISTIC(TotalMul, "Number of mul");

static void countInnerLoops(Loop * loops){
  TotalLoops++;
  for (Loop::iterator l = loops->begin(), e = loops->end(); l != e; ++l) { countInnerLoops(*l);}
  return;
}

PreservedAnalyses VokhmyaninaCounter::run(Function &F, FunctionAnalysisManager &AM) {
  if (!F.isDeclaration()) {
    TotalFuncsDef++;
    llvm::LoopAnalysis::Result& loops = AM.getResult<LoopAnalysis>(F);
    for (LoopInfo::iterator l = loops.begin(), e = loops.end(); l != e; ++l) { countInnerLoops(*l); }
    for (Function::iterator b = F.begin(), f_e = F.end(); b != f_e; ++b) {
      TotalBasicBlocks++;
      for (BasicBlock::iterator i = b->begin(), b_e = b->end(); i != b_e; ++i) {
        if (i->getOpcode() == Instruction::Add || i->getOpcode() == Instruction::FAdd) { TotalAdd++;}
        if (i->getOpcode() == Instruction::Mul || i->getOpcode() == Instruction::FMul) { TotalMul++;}
      }
    }
  }
  return PreservedAnalyses::all();
  
}
