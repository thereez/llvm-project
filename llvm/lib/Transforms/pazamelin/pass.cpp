#include "llvm/Transforms/pazamelin/pass.h"

using namespace llvm;

PreservedAnalyses PAZamelinPass::run(Function& F, FunctionAnalysisManager& AM)
{
    using bitc::BinaryOpcodes;

    if (!F.isDeclaration())
    {
        TotalFunctions++;
        TotalBasicBlocks += F.size();
    
        LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
        for (const auto* L : LI)
        {
            handleLoop(L);
        }

        for (const BasicBlock& B : F)
        {
            for (const Instruction& I : B)
            {
                if (I.isBinaryOp())
                {
                    const auto opcode = I.getOpcode();  
                    switch (opcode)
                    {
                        case Instruction::Add:
                        case Instruction::FAdd:
                        {
                            TotalAdd++;    
                            break;
                        }                    
                        case Instruction::Mul:
                        case Instruction::FMul:
                        {
                            TotalMul++;
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        }
    }    

    return PreservedAnalyses::all();
}

void PAZamelinPass::handleLoop(const Loop* L) const
{
    TotalLoops++;
    for (const auto* SL : L->getSubLoops())
    {
        handleLoop(SL);
    }
}