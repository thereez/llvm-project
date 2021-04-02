#include "llvm/Transforms/IlyinPass2/IlyinPass2.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

PreservedAnalyses IlyinPass2::run(Function &F, FunctionAnalysisManager &AM) {

  auto pa = PreservedAnalyses::all();

  SmallVector<BinaryOperator *, 16> right_set;
  SmallVector<BinaryOperator *, 16> left_set;
  SmallVector<CallInst *, 16> pows_set;

  for (auto &I : instructions(F)) {

    if (auto BO = dyn_cast<BinaryOperator>(&I)) {

      if (auto RHSC_left = dyn_cast<ConstantFP>(BO->getOperand(0))) {

        if (RHSC_left->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) {

          left_set.push_back(BO);
          pa = PreservedAnalyses::none();

        }

        if (RHSC_left->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) {

          left_set.push_back(BO);
          pa = PreservedAnalyses::none();

        }

      } 

	  else if (auto RHSC_right = dyn_cast<ConstantFP>(BO->getOperand(1))) { 

		if (RHSC_right->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) {

          right_set.push_back(BO);
          pa = PreservedAnalyses::none();

        }

        if (RHSC_right->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) {

          right_set.push_back(BO);
          pa = PreservedAnalyses::none();

        }

	  }

    }

    if (auto CI = dyn_cast<CallInst>(&I)) {

      if (CI->getCalledFunction()->getName() == "powf") {

        if (auto RHSC = dyn_cast<ConstantFP>(CI->getArgOperand(1))) {

          if (RHSC->getValue().convertToFloat() == 2.f) {

              pows_set.push_back(CI);
			  pa = PreservedAnalyses::none();     

		  }    

		}   

	  } 

	}

  }

  while (!left_set.empty()) {

    auto BO = left_set.pop_back_val();
    BO->replaceAllUsesWith(BO->getOperand(1));
    BO->eraseFromParent();

  }

  while (!right_set.empty()) {

    auto BO = right_set.pop_back_val();
    BO->replaceAllUsesWith(BO->getOperand(0));
    BO->eraseFromParent();

  }

  while (!pows_set.empty()) {

    auto CI = pows_set.pop_back_val();
    auto operand = CI->getOperand(0);
    auto BO = BinaryOperator::Create(Instruction::FMul, operand, operand);
    CI->replaceAllUsesWith(BO);
    BO->insertAfter(CI);
    CI->eraseFromParent();

  }

  return pa;
}