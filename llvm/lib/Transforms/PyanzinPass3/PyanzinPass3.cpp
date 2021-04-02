#include "llvm/Transforms/PyanzinPass3/PyanzinPass3.h"
#define DEBUG_TYPE "pyanzinpass3"

using namespace llvm;

STATISTIC(TotalFuncs, "Number of functions");
STATISTIC(TotalBasicBlocks, "Number of basic blocks");
STATISTIC(TotalAddInstrs, "Number of add instructions");
STATISTIC(TotalMulInstrs, "Number of mul instructions");
STATISTIC(TotalLoopsVectorizableIV, "Number of intermost loops in canonical form with a single induction variable");


void handleLoop(Loop *L, LoopAnalysisManager& LAM, LoopStandardAnalysisResults& AR)
{
    if(L->isInnermost())
    {
        auto& PLA = LAM.getResult<PyanzinLoopAnalysis>(*L, AR);
        errs() << "Updates count:" << PLA.updates_count << "\n";
        if (PLA.updates_count == 1)
        {
            TotalLoopsVectorizableIV++;
        }
    }

    for (Loop *SL : L->getSubLoops())
    {
        handleLoop(SL, LAM, AR);
    }
}

PreservedAnalyses PyanzinPass3::run(Function &F,
                                      FunctionAnalysisManager &AM) 
                                    {

  TotalFuncs++;

  auto& PA = AM.getResult<PyanzinAnalysis>(F);
  TotalBasicBlocks += PA.bb_count;
  TotalAddInstrs += PA.add_count;
  TotalMulInstrs += PA.mul_count;

  auto& LI = AM.getResult<LoopAnalysis>(F);
  auto& LAM = AM.getResult<LoopAnalysisManagerFunctionProxy>(F).getManager();

  auto& AA = AM.getResult<AAManager>(F);
  auto& AC = AM.getResult<AssumptionAnalysis>(F);
  auto& DT = AM.getResult<DominatorTreeAnalysis>(F);
  auto& SE = AM.getResult<ScalarEvolutionAnalysis>(F);
  auto& TLI = AM.getResult<TargetLibraryAnalysis>(F);
  auto& TTI = AM.getResult<TargetIRAnalysis>(F);

  LoopStandardAnalysisResults AR = {AA, AC, DT, LI, SE, TLI, TTI, nullptr, nullptr};
  for (auto& L : LI)
  {
      handleLoop(L, LAM, AR);

  }



  return PreservedAnalyses::all();
}
