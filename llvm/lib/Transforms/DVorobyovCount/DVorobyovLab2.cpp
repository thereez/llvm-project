#include "llvm/Transforms/DVorobyovCount/DVorobyovCount.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#define DEBUG_TYPE "dvorobyovcountpass"

#include "llvm/IR/InstIterator.h"
#include "llvm/ADT/SmallVector.h"
using namespace llvm;

PreservedAnalyses DVorobyovCountPass::run(Function &F, FunctionAnalysisManager &AM) 
{
	auto pa = PreservedAnalyses::all();
    SmallVector<BinaryOperator *, 16> wark_set_left;
    SmallVector<BinaryOperator *, 16> wark_set_right;
    SmallVector<CallBase *, 16> wark_set_powf;

    for (auto &I : instructions(F))
    {
        if (auto BO = dyn_cast<BinaryOperator>(&I)) 
        {
            if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1))) 
            {
                if (RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) 
                {
                    wark_set_right.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
                if (RHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) 
                {
                    wark_set_right.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
			} else if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(0))) 
			{
				if (RHSC->getValue().convertToFloat() == 0.f &&  BO->getOpcode() == Instruction::FAdd) 
				{
					wark_set_left.push_back(BO);
					pa = PreservedAnalyses::none();
				}
				if (RHSC->getValue().convertToFloat() == 1.f &&  BO->getOpcode() == Instruction::FMul) 
				{
					wark_set_left.push_back(BO);
					pa = PreservedAnalyses::none();
				}
			}
		}
        if (auto C = dyn_cast<CallInst>(&I)) 
        {
            if (C->getCalledFunction()->getName() == "powf") 
            {
                if (auto PV = dyn_cast<ConstantFP>(C->getOperand(1))) 
                {
                    if (PV->getValue().convertToFloat() == 2.f) 
                    {
                        wark_set_powf.push_back(C);
                        pa = PreservedAnalyses::none();
                    }
                }
            }
        }
    }

    while (!wark_set_right.empty()) 
    {
        auto BO = wark_set_right.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(0));
        BO->eraseFromParent();
    }

    while (!wark_set_left.empty()) 
    {
        auto BO = wark_set_left.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(1));
        BO->eraseFromParent();
    }

    while (!wark_set_powf.empty()) 
    {
        auto C = wark_set_powf.pop_back_val();
        auto BO_MUL = BinaryOperator::Create(Instruction::FMul, C->getOperand(0), C->getOperand(0));
        C->replaceAllUsesWith(BO_MUL);
        BO_MUL->insertBefore(CB);
        C->eraseFromParent();
    }

    return pa;