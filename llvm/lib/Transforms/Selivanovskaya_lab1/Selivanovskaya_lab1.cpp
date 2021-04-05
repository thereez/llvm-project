#include "llvm/Transforms/Selivanovskaya_lab1/Selivanovskaya_lab1.h"

#define DEBUG_TYPE "Selivanovskaya_lab1"
#include "llvm/ADT/Statistic.h"

using namespace llvm;
using namespace std;

STATISTIC(FuncsNum, "Number of function definitions");
STATISTIC(BasicBlocksNum, "Number of basic blocks");
STATISTIC(AddsNum, "Number of adds");
STATISTIC(MulsNum, "Number of muls");
STATISTIC(LoopsNum, "Number of loops");

static void LoopsCount(Loop * loop){
    LoopsNum++;
    for (Loop::iterator curr_loop = loop->begin(); curr_loop != loop->end(); ++curr_loop) {
        LoopsCount(*curr_loop);
    }
}

PreservedAnalyses Selivanovskaya_lab1::run(Function &F, FunctionAnalysisManager &AM) {
    if (!F.isDeclaration()){
        FuncsNum++;

        LoopAnalysis::Result& loops = AM.getResult<LoopAnalysis>(F);
        for (LoopInfo::iterator loop = loops.begin(); loop != loops.end(); ++loop) {
            LoopsCount(*loop);
        }

        for (Function::iterator bb = F.begin(); bb != F.end(); ++bb) {
            BasicBlocksNum++;
            for (BasicBlock::iterator instr = bb->begin() ; instr != bb->end(); ++instr) {
                if (string(instr->getOpcodeName()) == "add" || string(instr->getOpcodeName()) == "fadd")
                    AddsNum++;
                if (string(instr->getOpcodeName()) == "mul" || string(instr->getOpcodeName()) == "fmul")
                    MulsNum++;
            }
        }
    }
    return PreservedAnalyses::all();
}


