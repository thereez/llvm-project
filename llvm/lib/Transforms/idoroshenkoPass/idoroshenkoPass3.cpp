//   write a pass Counting:
// - only function definitions
// - loops
// - Basic blocks
// - Arithmetic instructions of type add and mull.

#include "llvm/Transforms/idoroshenkoPass/idoroshenkoPass3.h"
#include "llvm/Analysis/IDoroshenkoAnalysis.h"
#include "llvm/Analysis/IDoroshenkoLoopAnalysis.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/TargetLibraryInfo.h"

#define DEBUG_TYPE "idoroshenkopass"
using namespace llvm;

STATISTIC(TotalPow, "Number of Pow()");
STATISTIC(TotalAdd, "Number of 'Add'");
STATISTIC(TotalMul, "Number of 'Mul'");
STATISTIC(TotalLoopsVectorizable, "Number of vectorizable loops");


void handleLoops(Loop* loop, LoopAnalysisManager& LAM, LoopStandardAnalysisResults& AR) {
    if (loop->isInnermost()) {
        auto& GLA = LAM.getResult<IDoroshenkoLoopAnalysis>(*loop, AR);
        if (GLA.InvUpdatesCount == 1) {
            ++TotalLoopsVectorizable;
        }
    }
    for (Loop* child : loop->getSubLoops()) {
        handleLoops(child, LAM, AR);
    }
}

PreservedAnalyses IDoroshenkoPass3::run(Function &F, FunctionAnalysisManager &AM) {

    auto& G = AM.getResult<IDoroshenkoAnalysis>(F);
    TotalAdd += G.TotalAdd;
    TotalMul += G.TotalMul;
    TotalPow += G.TotalPow;

    G.print(errs());

    auto& LI = AM.getResult<LoopAnalysis>(F);
    auto& LAM = AM.getResult<LoopAnalysisManagerFunctionProxy>(F).getManager();
    
    auto& AA = AM.getResult<AAManager>(F);
    AssumptionCache &AC = AM.getResult<AssumptionAnalysis>(F);
    DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
    ScalarEvolution &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
    TargetLibraryInfo &TLI = AM.getResult<TargetLibraryAnalysis>(F);
    TargetTransformInfo &TTI = AM.getResult<TargetIRAnalysis>(F);
    LoopStandardAnalysisResults AR = {AA , AC, DT, LI, SE, TLI, TTI, nullptr, nullptr};

    for (auto& L : LI)
        handleLoops(L, LAM, AR);

    return PreservedAnalyses::all();
}