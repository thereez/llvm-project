#include "llvm/Transforms/PankratovaPass/PankratovaPass.h"

using namespace llvm;


static void countLoops(const Loop* L) {
    totalLoops++;
    auto loops = L->getSubLoops();
    for (auto L = loops.begin(); L != loops.end(); L++) {
        countLoops(*L);
    }
}

PreservedAnalyses PankratovaPass::run(Function &Func, FunctionAnalysisManager &AM) {
    if (!Func.isDeclaration()) {
        totalDef++;
    }
    llvm::LoopAnalysis::Result& loops = AM.getResult<LoopAnalysis>(Func);
    for (auto L = loops.begin(); L != loops.end(); L++) {
        countLoops(*L);
    }
    for (BasicBlock &BB : Func) {
        totalBlocks++;
        for (Instruction &I : BB) {
            std::string instructionName = std::string(I.getOpcodeName());
            if (instructionName == "add" || instructionName == "fadd") {
                totalAdd++;
            }
            else if (instructionName == "mul" || instructionName == "fmul") {
                totalMul++;
            }
        }
    }
    return PreservedAnalyses::all();
}
