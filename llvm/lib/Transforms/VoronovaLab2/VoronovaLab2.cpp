#include "llvm/Transforms/VoronovaLab2/VoronovaLab2.h"


using namespace llvm;





PreservedAnalyses VoronovaLab2::run(Function &F, FunctionAnalysisManager &AM) {
	auto pa = PreservedAnalyses::all();
	SmallVector<BinaryOperator*, 16> wark_set_add_right;
    SmallVector<BinaryOperator *, 16> wark_set_mul_right;
	SmallVector<BinaryOperator*, 16> wark_set_add_left;
    SmallVector<BinaryOperator *, 16> wark_set_mul_left;
    SmallVector<CallInst*, 16> wark_set_pows;
	for (auto& I : instructions(F))
	{
		if (auto BO = dyn_cast<BinaryOperator>(&I))
		{
			if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1)))
			{
				if (RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) 
				{
					wark_set_add_right.push_back(BO);
					pa= PreservedAnalyses::none();
				}
                if (RHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) 
				{
                    wark_set_mul_right.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
			}

			if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(0)))
			{
				if (RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) 
				{
					wark_set_add_left.push_back(BO);
					pa= PreservedAnalyses::none();
				}
                if (RHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) 
				{
                    wark_set_mul_left.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
			}
		}

		if (auto POW = dyn_cast<CallInst>(&I))
		{
          if(POW->getCalledFunction()->getName() == "powf")
		  {
            if(auto arg = dyn_cast<ConstantFP>(POW->getOperand(1)))
			{
              if (arg->getValue().convertToFloat() == 2.f)
			  {
                wark_set_pows.push_back(POW);
                pa = PreservedAnalyses::none();
              }
            }
          }
        }
	}

	while (!wark_set_add_right.empty())
	{
		auto BO = wark_set_add_right.pop_back_val();
		BO->replaceAllUsesWith(BO->getOperand(0));
		BO->eraseFromParent();

	}

	while (!wark_set_mul_right.empty()) 
	{
          auto BO = wark_set_mul_right.pop_back_val();
          BO->replaceAllUsesWith(BO->getOperand(0));
          BO->eraseFromParent();
    }

	while (!wark_set_add_left.empty())
	{
		auto BO = wark_set_add_left.pop_back_val();
		BO->replaceAllUsesWith(BO->getOperand(1));
		BO->eraseFromParent();

	}

	while (!wark_set_mul_left.empty()) 
	{
        auto BO = wark_set_mul_left.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(1));
        BO->eraseFromParent();
    }

	while (!wark_set_pows.empty()) {
        auto POW = wark_set_pows.pop_back_val();
        auto NewMul = BinaryOperator::Create(Instruction::FMul, POW->getOperand(0), POW->getOperand(0));
        POW->replaceAllUsesWith(NewMul);
        NewMul->insertBefore(POW);
        POW->eraseFromParent();
    }
	

	return pa;
}