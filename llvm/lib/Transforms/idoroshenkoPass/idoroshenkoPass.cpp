//   write a pass Counting:
// - only function definitions
// - loops
// - Basic blocks
// - Arithmetic instructions of type add and mull.

#include "llvm/Transforms/idoroshenkoPass/idoroshenkoPass.h"

#define DEBUG_TYPE "idoroshenkopass"
using namespace llvm;

STATISTIC(TotalFuncsDef, "Number of function definitions");
STATISTIC(TotalLoops, "Number of loops");
STATISTIC(TotalBasicBlocks, "Number of basic blocks");
STATISTIC(TotalAdd, "Number of 'Add'");
STATISTIC(TotalMul, "Number of 'Mul'");

void countLoopsRecursive(Loop* loop) {
    TotalLoops++;
    for (Loop::iterator newLoop = loop->begin(); newLoop != loop->end(); ++newLoop) {
        countLoopsRecursive(*newLoop);
    }
}

static void countLoops(llvm::LoopAnalysis::Result& loops) {
    for (LoopInfo::iterator loop = loops.begin(); loop != loops.end(); ++loop) {
        countLoopsRecursive(*loop);
    }
}

PreservedAnalyses IDoroshenkoPass::run(Function &function, FunctionAnalysisManager &AM) {
    if(!function.isDeclaration()) {
        TotalFuncsDef++;
        llvm::LoopAnalysis::Result& loopsResult = AM.getResult<LoopAnalysis>(function);
        countLoops(loopsResult);
        for (Function::iterator basicBlock = function.begin(); basicBlock != function.end(); ++basicBlock) {
            TotalBasicBlocks++;
            for (BasicBlock::iterator instruction = basicBlock->begin() ; instruction != basicBlock->end(); ++instruction) {
                if (std::string(instruction->getOpcodeName()) == "add" || std::string(instruction->getOpcodeName()) == "fadd")
                    TotalAdd++;
                if (std::string(instruction->getOpcodeName()) == "mul" || std::string(instruction->getOpcodeName()) == "fmul")
                    TotalMul++;
            }
        }
    }

    return PreservedAnalyses::all();
}