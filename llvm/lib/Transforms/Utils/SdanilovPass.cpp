#include "llvm/Transforms/Utils/SdanilovPass.h"
#include <iostream>
using namespace llvm;

void SdanilovPass::countLoopsNested(Loop* loop){
  std::vector<Loop*> subLoop = loop->getSubLoops();
  LoopCounter+= subLoop.size();
}

PreservedAnalyses SdanilovPass::run(Function &F, FunctionAnalysisManager &AM){
  LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
  if (!F.isDeclaration()){
    ++FunctionCounter;
  }
  for (auto &BB: F){
    BasicBlocksCounter++;
    for (auto &I : BB){
      std::string opcodename = I.getOpcodeName();
      if (opcodename == "add" || opcodename == "mul"){
          AddMulCounter++;
      }
    }
  }
  for (LoopInfo::iterator i = LI.begin(), b = LI.end(); i!=b; ++i){
    LoopCounter++;
    Loop *loopPtr = *i;
    countLoopsNested(loopPtr);
  }
  return PreservedAnalyses::all();
};



