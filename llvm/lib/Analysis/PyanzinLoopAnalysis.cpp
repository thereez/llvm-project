#include "llvm/Analysis/PyanzinLoopAnalysis.h"
#include <set>

using namespace llvm;

AnalysisKey PyanzinLoopAnalysis::Key;

PyanzinLoopAnalysis::Result PyanzinLoopAnalysis::run(Loop& L, LoopAnalysisManager& LAM, LoopStandardAnalysisResults& LSA)
{
    int64_t updates_count = 0;

    if (PHINode* indVar = L.getCanonicalInductionVariable()) {
        errs() << "canonical ind variable found" << "\n";
    // collect all binary operators which update this induction variable
    // in ideal vectorizable case it should be a single binary operator which updates loop induction
        SmallVector<BinaryOperator*, 16> updates;
        for (unsigned int i = 0; i < indVar->getNumIncomingValues(); i++) {
            if (auto* binOp = dyn_cast<BinaryOperator>(indVar->getIncomingValue(i))) 
            {
                if (binOp->getOperand(0) == indVar || binOp->getOperand(1) == indVar) {
                    updates.push_back(binOp);
                }
            }
        }
        updates_count = updates.size();
        }
    Result res = {updates_count};
    return res;
}

