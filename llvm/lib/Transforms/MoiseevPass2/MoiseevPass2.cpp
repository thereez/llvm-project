#include "llvm/Transforms/MoiseevPass2/MoiseevPass2.h"

using namespace llvm;

PreservedAnalyses MoiseevPass2::run(Function &func, FunctionAnalysisManager &AM)
{
    auto PA = PreservedAnalyses::all();
    SmallVector<BinaryOperator*, 16> wark_left;
    SmallVector<BinaryOperator*, 16> wark_right;
    SmallVector<CallBase*, 16> wark_pow;

    for (auto& I : instructions(func)) {
        if (auto boper = dyn_cast<BinaryOperator>(&I)){
            if(auto right = dyn_cast<ConstantFP>(boper->getOperand(1))) {
                if(right->getValue().convertToFloat() == 0.f && boper->getOpcode() == Instruction::FAdd) {
                    wark_right.push_back(boper);
                    PA = PreservedAnalyses::none();
                }
                
                if(right->getValue().convertToFloat() == 1.f && boper->getOpcode() == Instruction::FMul) {
                    wark_right.push_back(boper);
                    PA = PreservedAnalyses::none();
                }

            }
            else if(auto left = dyn_cast<ConstantFP>(boper->getOperand(0))) 
            {
                if(left->getValue().convertToFloat() == 0.f && boper->getOpcode() == Instruction::FAdd) {
                    wark_left.push_back(boper);
                    PA = PreservedAnalyses::none();
                }

                if(left->getValue().convertToFloat() == 1.f && boper->getOpcode() == Instruction::FMul) {
                    wark_left.push_back(boper);
                    PA = PreservedAnalyses::none();
                } 
            }
        }
        if (auto callinst = dyn_cast<CallInst>(&I)){
          if(callinst->getCalledFunction()->getName() == "powf"){
            if(auto arg = dyn_cast<ConstantFP>(callinst->getOperand(1))){
              if (arg->getValue().convertToFloat() == 2.f){
                wark_pow.push_back(callinst);
                PA = PreservedAnalyses::none();
              }
            }
          }
        }
    }

    while (!wark_right.empty()) {
        auto boper = wark_right.pop_back_val();
        boper->replaceAllUsesWith(boper->getOperand(0));
        boper->eraseFromParent();
    }

    while (!wark_left.empty()) {
        auto boper = wark_left.pop_back_val();
        boper->replaceAllUsesWith(boper->getOperand(1));
        boper->eraseFromParent();
    }

    while (!wark_pow.empty()) {
        auto callinst = wark_pow.pop_back_val();
        auto mul = BinaryOperator::Create(Instruction::FMul, callinst->getOperand(0), callinst->getOperand(0));
        callinst->replaceAllUsesWith(mul);
        mul->insertBefore(callinst);
        callinst->eraseFromParent();
    }

    return PA;
}