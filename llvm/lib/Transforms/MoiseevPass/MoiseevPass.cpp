#include "llvm/Transforms/MoiseevPass/MoiseevPass.h"
#define DEBUG_TYPE "MoiseevPass"

#include "llvm/ADT/Statistic.h"
using namespace llvm;

STATISTIC(TotalAdd, "Num of add");
STATISTIC(TotalMul, "Num of mul");
STATISTIC(TotalFuncsDef, "Num of func definitions");
STATISTIC(TotalLoops, "Num of loops");
STATISTIC(TotalBasicBlocks, "Num of basic blocks");


static void countLoops(Loop * loop){
    TotalLoops++;

    for (Loop::iterator loopIterator = loop->begin(); loopIterator != loop->end(); ++loopIterator){
        countLoops(*loopIterator);
    }

    return;
}

PreservedAnalyses MoiseevPass::run(Function &func, FunctionAnalysisManager &AM){
    if (!func.isDeclaration()){

        TotalFuncsDef++;
        llvm::LoopAnalysis::Result& loops = AM.getResult<LoopAnalysis>(func);

        for (LoopInfo::iterator loopInfoIterator = loops.begin(); loopInfoIterator != loops.end(); ++loopInfoIterator) {
            countLoops(*loopInfoIterator);
        }

        for (Function::iterator basicBlocks = func.begin(); basicBlocks != func.end(); ++basicBlocks){

            TotalBasicBlocks++;

            for (BasicBlock::iterator instr = basicBlocks->begin(); instr != basicBlocks->end(); ++instr){
                if (instr->getOpcode() == Instruction::Add || instr->getOpcode() == Instruction::FAdd){
                    TotalAdd++;
                }
                if (instr->getOpcode() == Instruction::Mul || instr->getOpcode() == Instruction::FMul){
                    TotalMul++;
                }
            }
        }
    }
    return PreservedAnalyses::all();
}
