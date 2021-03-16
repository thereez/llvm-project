//   write a pass Counting: 
// - only function definitions 
// - loops
// - Basic blocks 
// - Arithmetic instructions of type add and mull.

#include "llvm/Transforms/idoroshenkoPass/idoroshenkoPass2.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"

#define DEBUG_TYPE "idoroshenkopass2"
using namespace llvm;


PreservedAnalyses IDoroshenkoPass2::run(Function &function, FunctionAnalysisManager &AM) {
    
    auto pa = PreservedAnalyses::all();

    SmallVector<BinaryOperator*, 16> wark_set;
    SmallVector<CallInst*, 16> wark_set_pow;
    for (auto& I : instructions(function)) {
        if (auto BO = dyn_cast<BinaryOperator>(&I)){
            if(auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1))) {
                if(RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) {
                    wark_set.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
                if(RHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) {
                    wark_set.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
            }
        }
        if (auto C = dyn_cast<CallInst>(&I)) 
            if(C->getCalledFunction()->getName() == "powf") 
                if(auto RHSC = dyn_cast<ConstantFP>(C->getOperand(1)))
                    if(RHSC->getValue().convertToFloat() == 2.f) {
                        wark_set_pow.push_back(C);
                        pa = PreservedAnalyses::none();
                    }
    }

    while (!wark_set.empty()) {
        auto BO = wark_set.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(0));
        BO->eraseFromParent(); 
    }

    while (!wark_set_pow.empty()) {
        auto C = wark_set_pow.pop_back_val();
        auto mul = BinaryOperator::Create(Instruction::FMul, C->getOperand(0), C->getOperand(0));
        C->replaceAllUsesWith(mul);
        mul->insertBefore(C);
        C->eraseFromParent();
    }

    return pa;
}
