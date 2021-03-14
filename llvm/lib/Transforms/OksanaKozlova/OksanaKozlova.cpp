//This is a pass, that counting:
// Only function definitions,
// Loops,
// Basic blocks,
// Arithmetic instructions of type add and mull.

#include "llvm/Transforms/OksanaKozlova/OksanaKozlova.h"

#define DEBUG_TYPE "oksanakozlovapass"

using namespace llvm;

STATISTIC(TotalFuncs, "Number of functions");
STATISTIC(TotalLoops, "Number of loops");
STATISTIC(TotalBasicBlocks, "Number of basic blocks");
STATISTIC(TotalArithmInstr, "Number of arithmetic instructions");

void LoopCounter(Loop* L) {
    TotalLoops++;
    for (auto loop = L->begin(); loop != L->end(); ++loop) {
        LoopCounter(*loop);
    }
  return;
}

PreservedAnalyses OksanaKozlovaPass::run(Function &F, FunctionAnalysisManager &AM) {
  if (!F.isDeclaration()) {
    errs() << F.getName() << "\n";
    TotalFuncs++;
  }
  llvm::LoopAnalysis::Result& L = AM.getResult<LoopAnalysis>(F);
  for (auto loop = L.begin(); loop != L.end(); ++loop) {
        LoopCounter(*loop); 
  }
  for (auto B = F.begin(); B != F.end(); ++B) {
    TotalBasicBlocks++;
    for (auto I = B->begin(); I != B->end(); ++I) {
      std::string instr = std::string(I->getOpcodeName());
      if (instr == "add" || instr == "fadd" || instr == "mul" || instr == "fmul") { 
        TotalArithmInstr++; 
      }
    }
  }
  return PreservedAnalyses::all();
}
