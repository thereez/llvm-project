#include "llvm/Transforms/AKomyaginCount/AKomyaginCount.h"
#define DEBUG_TYPE "AKomyaginCount"
#include "llvm/ADT/Statistic.h"
STATISTIC(FunctionCount, "Number of function definitions");
STATISTIC(LoopsCount, "Number of loops");
STATISTIC(BlocksCount, "Number of basic blocks");
STATISTIC(MulCount, "Number of mul");
STATISTIC(AddCount, "Number of add");
using namespace llvm;

static void countInnerLoops(Loop* loop) {
    ++LoopsCount;
    for (Loop* innerLoop : loop->getSubLoops()) {
        countInnerLoops(innerLoop);
    }
}

PreservedAnalyses AKomyaginCount::run(Function& F, FunctionAnalysisManager& AM) {

    if (!F.isDeclaration()) {
        ++FunctionCount;
    }

    LoopAnalysis::Result& Loops = AM.getResult<LoopAnalysis>(F);
    for (Loop* iter : Loops) {
        countInnerLoops(iter);
    }

    for (BasicBlock& iter1 : F) {
        ++BlocksCount;
        for (Instruction& iter2 : iter1) {
            if (std::string(iter2.getOpcodeName()) == "mul" || std::string(iter2.getOpcodeName()) == "fmul") {
                ++MulCount;
            }
            else if (std::string(iter2.getOpcodeName()) == "add" || std::string(iter2.getOpcodeName()) == "fadd") {
                ++AddCount;
            }
        }
    }

    return PreservedAnalyses::all();
}
