#include "llvm/Transforms/AKomyaginCount/AKomyaginCount.h"

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
