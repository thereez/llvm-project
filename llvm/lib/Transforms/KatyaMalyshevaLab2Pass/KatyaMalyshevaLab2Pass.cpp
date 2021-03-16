#include "llvm/Transforms/KatyaMalyshevaLab2Pass/KatyaMalyshevaLab2Pass.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#define DEBUG_TYPE "KatyaMalyshevaLab2Pass" 

using namespace llvm;

PreservedAnalyses KatyaMalyshevaLab2Pass::run(Function& f, FunctionAnalysisManager& AM) {
    auto pa = PreservedAnalyses::all();
	SmallVector<BinaryOperator*, 16> wark_set_r;
	SmallVector<BinaryOperator*, 16> wark_set_l;
	SmallVector<CallInst*, 16> Ci_powf;


	for (auto& I : instructions(f)) {
		if (auto BO = dyn_cast<BinaryOperator>(&I)) {
			if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(0))) {// left operand
				//f0 and fadd or f1 and fmul
				if ((RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) ||( RHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) ){
					wark_set_l.push_back(BO);
					pa = PreservedAnalyses::none();
				}
			}
			if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1))) {// right operand
				//f0 and fadd or f1 and fmul
				if ((RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) || (RHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul)) {
					wark_set_r.push_back(BO);
					pa = PreservedAnalyses::none();
		
				}

			}
		}
		else if (auto CI = dyn_cast<CallInst>(&I)) {
			if (CI->getCalledFunction()->getName() == StringRef("powf")) {
				if (auto deg = dyn_cast<ConstantFP>(CI->getArgOperand(1))) {
					if (deg->getValue().convertToFloat() == 2.f) {
						Ci_powf.push_back(CI);
						pa = PreservedAnalyses::none();
					}
				}
			}
		}


	}
	while (!wark_set_l.empty()) {
		auto BO = wark_set_l.pop_back_val();
		BO->replaceAllUsesWith(BO->getOperand(1));
		BO->eraseFromParent();
	}
	while (!wark_set_r.empty()) {
		auto BO = wark_set_r.pop_back_val();
		BO->replaceAllUsesWith(BO->getOperand(0));
		BO->eraseFromParent();
	}
	while (!Ci_powf.empty()) {
		auto Ci = Ci_powf.pop_back_val();
		// create fmul instead of powf
		auto mul = BinaryOperator::Create(Instruction::FMul, Ci->getOperand(0), Ci->getOperand(0));
		Ci->replaceAllUsesWith(mul);
		mul->insertBefore(Ci);
		Ci->eraseFromParent();
	}

	return pa;
}