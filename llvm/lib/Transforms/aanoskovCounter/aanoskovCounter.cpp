#include "llvm/Transforms/aanoskovCounter/aanoskovCounter.h"

#define DEBUG_TYPE "aanoskovCounter"
using namespace llvm;

STATISTIC(TotalFuncDefs, "Number of function definitions");
STATISTIC(TotalBasicBlocks, "Number of basic blocks");
STATISTIC(TotalAdds, "Number of Adds");
STATISTIC(TotalMuls, "Number of Muls");
STATISTIC(TotalLoops, "Number of loops");

void LoopsRecursion(Loop* loop)
{
    TotalLoops++;
    for(Loop::iterator innerLoop = loop->begin(); innerLoop != loop->end(); ++innerLoop)
    {
        LoopsRecursion(*innerLoop);
    }
}

PreservedAnalyses aanoskovCounter::run(Function &F,
                                      FunctionAnalysisManager &AM) {
    errs() << F.getName() << "\n";
    if (!F.isDeclaration())
        TotalFuncDefs++;
    
    LoopAnalysis::Result& loops = AM.getResult<LoopAnalysis>(F);   
    for(LoopInfo::iterator loop = loops.begin(); loop != loops.end(); ++loop)
    {
        LoopsRecursion(*loop);
    }

    for(Function::iterator bb = F.begin(); bb != F.end(); ++bb)
    {
        TotalBasicBlocks++;
        for(BasicBlock::iterator instr = bb->begin(); instr != bb->end(); ++instr)
        {
            if ( ( instr->getOpcode() == Instruction::Add ) || ( instr->getOpcode() == Instruction::FAdd ))
                TotalAdds++;
            else if ( ( instr->getOpcode() == Instruction::Mul ) || ( instr->getOpcode() == Instruction::FMul))
                TotalMuls++;
        }
    }
    return PreservedAnalyses::all();
}
