#include "llvm/Transforms/VokhmyaninaCounter/VokhmyaninaCounter.h"

using namespace llvm;

// STATISTIC(TotalFuncsDef, "Number of function definitions");
// STATISTIC(TotalLoops, "Number of loops");
// STATISTIC(TotalBasicBlocks, "Number of basic blocks");
// STATISTIC(TotalAdd, "Number of add");
// STATISTIC(TotalMul, "Number of mul");

// void countInnerLoops(Loop * loops){
//   TotalLoops++;
//   for (Loop::iterator l = loops->begin(), e = loops->end(); l != e; ++l) { countInnerLoops(*l);}
//   return;
// }

PreservedAnalyses VokhmyaninaCounter::run(Function &F, FunctionAnalysisManager &AM) {
  // if (!F.isDeclaration()) {
  //   TotalFuncsDef++;
  //   llvm::LoopAnalysis::Result& loops = AM.getResult<LoopAnalysis>(F);
  //   for (LoopInfo::iterator l = loops.begin(), e = loops.end(); l != e; ++l) { countInnerLoops(*l); }
  //   for (Function::iterator b = F.begin(), f_e = F.end(); b != f_e; ++b) {
  //     TotalBasicBlocks++;
  //     for (BasicBlock::iterator i = b->begin(), b_e = b->end(); i != b_e; ++i) {
  //       if (i->getOpcode() == Instruction::Add || i->getOpcode() == Instruction::FAdd) { TotalAdd++;}
  //       if (i->getOpcode() == Instruction::Mul || i->getOpcode() == Instruction::FMul) { TotalMul++;}
  //     }
  //   }
  // }
  // return PreservedAnalyses::all();

  auto pa = PreservedAnalyses::all();
 
    SmallVector<BinaryOperator*, 16> wark_set_R;
    SmallVector<BinaryOperator*, 16> wark_set_L;
    SmallVector<CallInst*, 16> wark_set_P;
    
    for (auto& I : instructions(F)) {
        if (auto BO = dyn_cast<BinaryOperator>(&I)){
            if(auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1))) {
                if(RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) {
                    wark_set_R.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
                if(RHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) {
                    wark_set_R.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
                 
            }
            else if(auto LHSC = dyn_cast<ConstantFP>(BO->getOperand(0))) {
                if(LHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) {
                    wark_set_L.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
                if(LHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) {
                    wark_set_L.push_back(BO);
                    pa = PreservedAnalyses::none();
                } 
            }
        }
        if (auto CI = dyn_cast<CallInst>(&I)){
          if(CI->getCalledFunction()->getName() == "powf"){
            if(auto arg = dyn_cast<ConstantFP>(CI->getOperand(1))){
              if (arg->getValue().convertToFloat() == 2.f){
                wark_set_P.push_back(CI);
                pa = PreservedAnalyses::none();
              }
            }
          }
        }
    }

    while (!wark_set_R.empty()) {
        auto BO = wark_set_R.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(0));
        BO->eraseFromParent();
    }
    while (!wark_set_L.empty()) {
        auto BO = wark_set_L.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(1));
        BO->eraseFromParent();
    }
    while (!wark_set_P.empty()) {
        auto CI = wark_set_P.pop_back_val();
        auto mul = BinaryOperator::Create(Instruction::FMul, CI->getOperand(0), CI->getOperand(0));
        CI->replaceAllUsesWith(mul);
        mul->insertBefore(CI);
        CI->eraseFromParent();
    }
 
    return pa;
}
