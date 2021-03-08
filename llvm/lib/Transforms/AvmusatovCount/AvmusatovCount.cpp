#include "llvm/Transforms/AvmusatovCount/AvmusatovCount.h"

using namespace llvm;

void countOperations(BasicBlock& basicBlock) {
    for (Instruction& instr : basicBlock) {
        std::string opcodeName = std::string(instr.getOpcodeName());

        if (opcodeName == "mul" || opcodeName == "fmul") {
            ++mulOperationCount;
        }
        else if (opcodeName == "add" || opcodeName == "fadd") {
            ++addOperationCount;
        }
    }
}

void countInnerLoops(Loop* loop) {
    ++loopsCount;
    for (Loop* innerLoop : loop->getSubLoops()) {
        countInnerLoops(innerLoop);
    }
}

PreservedAnalyses AvmusatovCount::run(Function& F, FunctionAnalysisManager& AM) {

    if (!F.isDeclaration()) {
        ++funcDefCount;
    }

    LoopAnalysis::Result& loops = AM.getResult<LoopAnalysis>(F);
    for (Loop* l : loops) {
        countInnerLoops(l);
    }

    for (BasicBlock& basicBlock : F) {
        ++basicBlocksCount;
        countOperations(basicBlock);
    }

    return PreservedAnalyses::all();
}