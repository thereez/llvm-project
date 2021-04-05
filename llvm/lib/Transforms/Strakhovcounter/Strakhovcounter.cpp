#include "llvm/Transforms/Strakhovcounter/Strakhovcounter.h"

#define DEBUG_TYPE "Strakhovcounter"
#include "llvm/ADT/Statistic.h"

using namespace std;
using namespace llvm;

STATISTIC(cntloop, "Number of loops");
STATISTIC(cntfunc, "Number of function definitions");
STATISTIC(cntbasicblock, "Number of basic blocks");
STATISTIC(cntadd, "Number of adds");
STATISTIC(cntmul, "Number of muls");


static void LoopsCount(Loop *loop) {
  cntloop++;
  for (Loop::iterator curr_loop = loop->begin(); curr_loop != loop->end();
       ++curr_loop) {
    LoopsCount(*curr_loop);
  }
}

PreservedAnalyses Strakhovcounter::run(Function &F, FunctionAnalysisManager &AM) {
  if (!F.isDeclaration()) {
    cntfunc++;

    LoopAnalysis::Result &loops = AM.getResult<LoopAnalysis>(F);
    for (LoopInfo::iterator loop = loops.begin(); loop != loops.end(); ++loop) {
      LoopsCount(*loop);
    }

    for (Function::iterator bb = F.begin(); bb != F.end(); ++bb) {
      cntbasicblock++;
      for (BasicBlock::iterator instr = bb->begin(); instr != bb->end();
           ++instr) {
        if (string(instr->getOpcodeName()) == "add" ||
            string(instr->getOpcodeName()) == "fadd")
          cntadd++;
        if (string(instr->getOpcodeName()) == "mul" ||
            string(instr->getOpcodeName()) == "fmul")
          cntmul++;
      }
    }
  }
  return PreservedAnalyses::all();
}