#include "llvm/Transforms/Bogoroditskaya2/Bogoroditskaya2.h"

#define DEBUG_TYPE "Bogoroditskaya2"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"

#include "llvm/ADT/Statistic.h"

using namespace llvm;

PreservedAnalyses Bogoroditskaya2::run(Function &F, FunctionAnalysisManager &AM)
{
    auto pa = PreservedAnalyses::all();
  // iterate over instructions to find candidates

  SmallVector<BinaryOperator*, 16> wark_set_left;
  SmallVector<BinaryOperator*, 16> wark_set_right;
  SmallVector<CallInst*, 16> call_inst;

  for (auto& I : instructions(F)) //чтобы не  итерироваться отдельно по блокам и итераторма внутри, используем специальный хедер
  { 
    if (auto BO = dyn_cast<BinaryOperator>(&I)) 
    {
      if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1))) // x * y.f
      { 
        if (RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) 
        {
          wark_set_left.push_back(BO);
          pa = PreservedAnalyses::none();
        }
        else if (RHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul)
        {
         wark_set_left.push_back(BO);
          pa = PreservedAnalyses::none();      
        }
      }

      else if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(0))) // y.f * x
      { 
        if (RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) 
        {
          wark_set_right.push_back(BO);
          pa = PreservedAnalyses::none();
        }
        else if (RHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul)
        {
          wark_set_right.push_back(BO);
          pa = PreservedAnalyses::none();      
        }        
      }
    }

    if (auto CallI = dyn_cast<CallInst>(&I))
    {
        if (CallI->getCalledFunction()->getName() == "powf")
        {
            if (auto i = dyn_cast<ConstantFP>(CallI->getOperand(1)))
            {
                if (i->getValue().convertToFloat() == 2.f)
                {
                    pa = PreservedAnalyses::none();
                    call_inst.push_back(CallI);
                }
            }
        }
    }
  }

  // remove redundant operations
  while (!wark_set_left.empty()) 
  {
    // entry:
    // t0 = x + 0.f
    // ret t0
    auto BO = wark_set_left.pop_back_val(); // t0 = x + 0.f
    BO->replaceAllUsesWith(BO->getOperand(0)); // x
    // entry:
    // t0 = x + 0.f
    // ret x
    BO->eraseFromParent();
    // entry:
    // ret x
  }

  while (!wark_set_right.empty()) 
  {
    auto BO = wark_set_right.pop_back_val(); // t0 = x + 0.f
    BO->replaceAllUsesWith(BO->getOperand(1)); // x
    BO->eraseFromParent();
  }

  while (!call_inst.empty()) 
  {
      auto CallI = call_inst.pop_back_val();
      auto x = BinaryOperator::Create(Instruction::FMul, CallI->getOperand(0), CallI->getOperand(0));
      CallI->replaceAllUsesWith(x);
      x->insertBefore(CallI);
      CallI->eraseFromParent();
  }  

  return pa;
}
