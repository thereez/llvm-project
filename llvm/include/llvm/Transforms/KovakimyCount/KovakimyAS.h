#pragma once

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"

namespace llvm
{

class LPMUpdater;	
	
class KovakimyPass : public PassInfoMixin<KovakimyPass>
{
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired()
  { 
    return true;
  }
};
}