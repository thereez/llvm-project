#include "llvm/Transforms/VolokhPass/VolokhPass.h"
#define DEBUG_TYPE "volokhpass"
using namespace llvm;

STATISTIC(TotalFuncsDef, "Num of funcs definitions");
STATISTIC(TotalLoops, "Num of loops");
STATISTIC(TotalBasicBlocks, "Num of basic blocks");
STATISTIC(TotalAdd, "Num of add operators");
STATISTIC(TotalMul, "Num of multiply operators");

static void countLoops(Loop * loop){ // rec cnt via iterator
    TotalLoops++;
    for (Loop::iterator loopIterator = loop->begin(); loopIterator != loop->end(); ++loopIterator){
        countLoops(*loopIterator);
    }
    return;
}

PreservedAnalyses VolokhPass::run(Function &func, FunctionAnalysisManager &AM){
    if (!func.isDeclaration()){
        TotalFuncsDef++;
        llvm::LoopAnalysis::Result& loops = AM.getResult<LoopAnalysis>(func);

        // inner loops
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