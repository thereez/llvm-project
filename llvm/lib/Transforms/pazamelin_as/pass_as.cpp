#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"

#define DEBUG_TYPE "pazamelin_as"

#include "llvm/Transforms/pazamelin_as/pass_as.h"

using namespace llvm;

PreservedAnalyses PAZamelinPassAS::run(Function& F,
                                       FunctionAnalysisManager& AM)
{
    auto pa = PreservedAnalyses::all();

    SmallVector<BinaryOperator*, 32> as_lhs;
    SmallVector<BinaryOperator*, 32> as_rhs;
    SmallVector<CallBase*, 32> sr_pow;

    // iterate over instructions to find candidates
    for (auto& I : instructions(F))
    {
        if (auto BO = dyn_cast<BinaryOperator>(&I))
        {
            if (BO->getOpcode() == Instruction::FMul)
            {
                bool lhs_is_one = false;
                bool rhs_is_one = false;

                // check instruction's left operand
                if (auto LHSC = dyn_cast<ConstantFP>(BO->getOperand(0)))
                {
                    lhs_is_one = (LHSC->getValue().convertToFloat() == 1.f);
                }

                // check instruction's right operand
                if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1)))
                {
                    rhs_is_one = (RHSC->getValue().convertToFloat() == 1.f);
                }

                // add candidate if appropriate
                if (lhs_is_one)
                {
                    as_lhs.push_back(BO);
                }
                else if (rhs_is_one)
                {
                    as_rhs.push_back(BO);
                }
            }
            else if (BO->getOpcode() == Instruction::FAdd)
            {
                bool lhs_is_zero = false;
                bool rhs_is_zero = false;

                // check instruction's left operand
                if (auto LHSC = dyn_cast<ConstantFP>(BO->getOperand(0)))
                {
                    lhs_is_zero = (LHSC->getValue().convertToFloat() == 0.f);
                }

                // check instruction's right operand
                if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1)))
                {
                    rhs_is_zero = (RHSC->getValue().convertToFloat() == 0.f);
                }

                // add candidate if appropriate
                if (lhs_is_zero)
                {
                    as_lhs.push_back(BO);
                }
                else if (rhs_is_zero)
                {
                    as_rhs.push_back(BO);
                }
            }            
        }
        else if (auto call = dyn_cast<CallBase>(&I))
        {
        	if(call->getCalledFunction()->getName()=="powf")
        	{
                if(auto RHSC = dyn_cast<ConstantFP>(call->getArgOperand(1)))
                { 
                    if(RHSC->getValue().convertToFloat()==2.f)
                    {
                        sr_pow.push_back(call);
                    }
                }
            }
        }
    }

    // invalidate analyses if needed
    if (!as_lhs.empty() or !as_rhs.empty() or !sr_pow.empty())
    {
        pa = PreservedAnalyses::none();
    }

    // remove redundant operations
    auto remover = [](auto& operations, bool side)
    {
        for (auto BO : operations)
        {
            // replace with an operand on the opposite side
            BO->replaceAllUsesWith(BO->getOperand(!side));
            BO->eraseFromParent();
        }
        operations.clear();
    };

    remover(as_lhs, false);
    remover(as_rhs, true);


    for (auto C : sr_pow)
    {
        const auto& operand = C->getArgOperand(0);
        auto BO = BinaryOperator::Create(Instruction::FMul, operand, operand);
        C->replaceAllUsesWith(BO);
        BO->insertAfter(C);
        C->eraseFromParent();    	
    }
	sr_pow.clear();    

    return pa;	
}
