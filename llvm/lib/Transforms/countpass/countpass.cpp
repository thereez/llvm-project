#define DEBUG_TYPE "CountPass"

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"

#include <cstring>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

STATISTIC(TotalFuncs,  "Number of functions");
STATISTIC(TotalMulls,  "Number of mull");
STATISTIC(TotalSums,  "Number of sum");
STATISTIC(TotalLoops,  "Number of loops");
STATISTIC(TotalBasicBlocks,  "Number of Basic Blocks");

using namespace llvm;

namespace {
struct CountPass : public FunctionPass {
  static char ID;
  CountPass() : FunctionPass(ID) {}
  void getAnalysisUsage(AnalysisUsage &AU) const override{
      AU.addRequired<LoopInfoWrapperPass>();
      AU.setPreservesAll();
  }

  void iterateOnLoops(Loop *L){
        TotalLoops++;     
        auto subLoops = L->getSubLoops();
        for(auto it_inside_loop = subLoops.begin(); it_inside_loop != subLoops.end(); ++it_inside_loop)
        iterateOnLoops(*it_inside_loop);
  }

  bool runOnFunction(Function &F) override {

    if(!F.isDeclaration()) TotalFuncs++;
    
    LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
    for(auto it_l = LI.begin(); it_l != LI.end(); ++it_l){
        iterateOnLoops(*it_l);
    }

    for(auto it_BB = F.begin(); it_BB != F.end(); ++it_BB){
            TotalBasicBlocks++;
            for(auto it_instr = it_BB->begin(); it_instr != it_BB->end(); ++it_instr){

                auto tmp = it_instr->getOpcodeName();
            //    errs() << tmp << "\n";

                if(strcmp(tmp, "fmul" )==0|| strcmp(tmp, "mul")==0) TotalMulls++;

                else if(strcmp(tmp, "fadd")==0 || strcmp(tmp, "add")==0) TotalSums++;
        }
    }

    return false;
  }
}; // end of struct CountPass
}  // end of anonymous namespace

char CountPass::ID = 0;
static RegisterPass<CountPass> X("CountPass", "CountPass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
