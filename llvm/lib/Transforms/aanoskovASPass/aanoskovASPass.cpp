#include "llvm/Transforms/aanoskovASPass/aanoskovASPass.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#define DEBUG_TYPE "aanoskovASPass"
#include "llvm/ADT/Statistic.h"

using namespace llvm;


PreservedAnalyses aanoskovASPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
   
    auto pa = PreservedAnalyses::all();

    SmallVector<BinaryOperator*, 16> wark_set_right;
    SmallVector<BinaryOperator*, 16> wark_set_left;
    SmallVector<CallInst*, 16> wark_set_pows;

    for (auto& I : instructions(F))
    {
        if (auto BO = dyn_cast<BinaryOperator>(&I))
        {
            if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1)))
            {
                if (RHSC->getValue().convertToFloat() == 0.f && (BO->getOpcode() == Instruction::FAdd) || (BO->getOpcode() == Instruction::Add))
                {
                    wark_set_right.push_back(BO);
                    pa = PreservedAnalyses::none();
                }

                else if (RHSC->getValue().convertToFloat() == 1.f && (BO->getOpcode() == Instruction::FMul) || (BO->getOpcode() == Instruction::Mul))
                {
                    wark_set_right.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
            }
            else if(auto LHSC = dyn_cast<ConstantFP>(BO->getOperand(0)))
            {
                if (LHSC->getValue().convertToFloat() == 0.f && (BO->getOpcode() == Instruction::FAdd) || (BO->getOpcode() == Instruction::Add))
                {
                    wark_set_left.push_back(BO);
                    pa = PreservedAnalyses::none();
                }

                else if (LHSC->getValue().convertToFloat() == 1.f && (BO->getOpcode() == Instruction::FMul) || (BO->getOpcode() == Instruction::Mul))
                {
                    wark_set_left.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
            }
        }
        if (auto CI = dyn_cast<CallInst>(&I))
        {
            if (auto f = CI->getCalledFunction())
            {
                if(f->getName() == "powf")
                {
                    if(auto power = dyn_cast<ConstantFP>(CI->getArgOperand(1)))
                    {
                        if(power->getValue().convertToFloat() == 2.f)
                        {
                            wark_set_pows.push_back(CI);
                            pa = PreservedAnalyses::none();
                        }
                    }
                }
            }
        }
    }

    while(!wark_set_right.empty())
    {
        auto BO = wark_set_right.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(0));
        BO->eraseFromParent();
    }
    while(!wark_set_left.empty())
    {
        auto BO = wark_set_left.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(1));
        BO->eraseFromParent();
    }

    while(!wark_set_pows.empty())
    {
        auto CI = wark_set_pows.pop_back_val();
        auto arg = CI->getArgOperand(0);
        auto inst = BinaryOperator::Create(Instruction::FMul, arg, arg);
        ReplaceInstWithInst(CI, inst);
    }
    return pa;
}
