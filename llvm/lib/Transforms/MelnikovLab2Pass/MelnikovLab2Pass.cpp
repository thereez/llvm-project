#include "llvm/Transforms/MelnikovLab2Pass/MelnikovLab2Pass.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#define DEBUG_TYPE "MelnikovLab2Pass" 

using namespace llvm;

PreservedAnalyses MelnikovLab2Pass::run(Function &func, FunctionAnalysisManager &AM) {
  auto pa = PreservedAnalyses::all();

  SmallVector<BinaryOperator*, 16> BOs_right_const; // binary operations, which right operand is a constant value (either 0 or 1)
  SmallVector<BinaryOperator*, 16> BOs_left_const; // binary operations, which left operand is a constant value
  SmallVector<CallInst*, 16> CIs_pow; // Call instructions, which call the powf function
  
  // iterating over instructions to find candidates
  for (auto& I : instructions(func)) {
    if (auto BO = dyn_cast<BinaryOperator>(&I)) {
      if (auto float_const = dyn_cast<ConstantFP>(BO->getOperand(0))) {
        if (float_const->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd || 
            float_const->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) 
        {
          BOs_left_const.push_back(BO);
          pa = PreservedAnalyses::none();
        }
      }

      else if (auto float_const = dyn_cast<ConstantFP>(BO->getOperand(1))) {
        if (float_const->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd ||
            float_const->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul)
        {
          BOs_right_const.push_back(BO);
          pa = PreservedAnalyses::none();
        }
      }
    }

    // llvm::CallInst represents a function call, abstracting a target machine's calling convention 
    else if (auto CI = dyn_cast<CallInst>(&I)) {
      if (CI->getCalledFunction()->getName() == StringRef("powf")) {
        //getArgOperand returns the i-th call argument
        if (auto float_const = dyn_cast<ConstantFP>(CI->getArgOperand(1))){
          if (float_const->getValue().convertToFloat() == 2.f){
            CIs_pow.push_back(CI);
            pa = PreservedAnalyses::none();
          }
        }
      }
    }
  }


  // removing redundant operations 
  while (!BOs_left_const.empty()) {    
    auto BO = BOs_left_const.pop_back_val(); 
    BO->replaceAllUsesWith(BO->getOperand(1)); 
    BO->eraseFromParent();    
  }

  while (!BOs_right_const.empty()) {    
    auto BO = BOs_right_const.pop_back_val(); 
    BO->replaceAllUsesWith(BO->getOperand(0)); 
    BO->eraseFromParent();    
  }

  while (!CIs_pow.empty()){
    auto CI = CIs_pow.pop_back_val();
    auto operand = CI->getOperand(0);

    BasicBlock::iterator ii(CI);
    auto BO = BinaryOperator::Create(Instruction::FMul, operand, operand);
    ReplaceInstWithInst(CI->getParent()->getInstList(), ii, BO); // holy f, it works 
    
  }
  return pa;
}
