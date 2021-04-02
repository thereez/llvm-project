
#include "llvm/Transforms/OksanaKozlova/OksanaAS.h"

#define DEBUG_TYPE "oksanaaspass"

using namespace llvm;
PreservedAnalyses OksanaASPass::run(Function &F, FunctionAnalysisManager &AM) {
	auto pa = PreservedAnalyses::all();
	// iterate over instructions to find candidates
	SmallVector<BinaryOperator*, 16> wark_set_right;
	SmallVector<BinaryOperator*, 16> wark_set_left;
	SmallVector<CallInst*, 16> powf_set;
	for (auto& I : instructions(F)) {

        if(auto CI = dyn_cast<CallInst>(&I)){
            if(CI->getCalledFunction()->getName()=="powf"){
                if(auto RHSC = dyn_cast<ConstantFP>(CI->getArgOperand(1))){
                    if(RHSC->getValue().convertToFloat()==2.f){
                        powf_set.push_back(CI);
                        pa = PreservedAnalyses::none();
                    }   
            	}
			}
		}

		if (auto BO = dyn_cast<BinaryOperator>(&I)) {
			if (auto RHSC_r = dyn_cast<ConstantFP>(BO->getOperand(1))) { // right const
				if (RHSC_r->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) {
					wark_set_right.push_back(BO);
					pa = PreservedAnalyses::none();
				}
				else if (RHSC_r->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) {
					wark_set_right.push_back(BO);
					pa = PreservedAnalyses::none();
				}

			} else if (auto RHSC_l = dyn_cast<ConstantFP>(BO->getOperand(0))) { // left const
				if (RHSC_l->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) {
					wark_set_right.push_back(BO);
					pa = PreservedAnalyses::none();
				}
				else if (RHSC_l->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) {
					wark_set_right.push_back(BO);
					pa = PreservedAnalyses::none();
				}
			}
		}
	}

	while (!powf_set.empty()) {
		auto CI = powf_set.pop_back_val();
		static LLVMContext TheContext;
		static IRBuilder<> Builder(TheContext);
    	auto CI_new = dyn_cast<BinaryOperator>(Builder.CreateFMul(CI->getArgOperand(0), CI->getArgOperand(0)));
		CI->replaceAllUsesWith(CI_new);
        CI_new->insertBefore(CI);
        CI->eraseFromParent();
	}

	// remove redundant operations
	while (!wark_set_left.empty()) {
		// entry:
		// t0 = 0.f + x
		// ret t0
		auto BO = wark_set_left.pop_back_val(); // t0 = 0.f + x
		BO->replaceAllUsesWith(BO->getOperand(1)); // x
		// entry:
		// t0 = 0.f + x
		// ret x
		BO->eraseFromParent();
		// entry:
		// ret x
	}

	// remove redundant operations
	while (!wark_set_right.empty()) {
		// entry:
		// t0 = x + 0.f
		// ret t0
		auto BO = wark_set_right.pop_back_val(); // t0 = x + 0.f
		BO->replaceAllUsesWith(BO->getOperand(0)); // x
		// entry:
		// t0 = x + 0.f
		// ret x
		BO->eraseFromParent();
		// entry:
		// ret x
	}

	// preserve analysis if none of instruction is removed
	return pa;

}