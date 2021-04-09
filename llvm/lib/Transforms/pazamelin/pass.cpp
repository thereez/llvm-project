#include "llvm/Transforms/pazamelin/pass.h"
#define DEBUG_TYPE "pazamelin"
#include "llvm/ADT/Statistic.h"
using namespace llvm;
STATISTIC(TotalFunctions, "Number of Functions");
STATISTIC(TotalBasicBlocks, "Number of BasicBlocks");
STATISTIC(TotalLoops, "Number of Loops");
STATISTIC(TotalAdd, "Number of Add");
STATISTIC(TotalMul, "Number of Mul");


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