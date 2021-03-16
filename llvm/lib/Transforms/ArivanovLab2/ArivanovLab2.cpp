#include "llvm/Transforms/ArivanovLab2/ArivanovLab2.h"

using namespace llvm;
using namespace std;

PreservedAnalyses ArivanovLab2Pass::run(Function &F, FunctionAnalysisManager &FAM) {
    auto pa = PreservedAnalyses::all();
    SmallVector<CallInst*, 16> IC_vec;
    SmallVector<BinaryOperator*, 16> RO_vec;
    SmallVector<BinaryOperator*, 16> LO_vec;
    for (auto& I : instructions(F)) {
        if (auto BO = dyn_cast<BinaryOperator>(&I)) {
            if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(0))) {
                if ((RHSC->getValue().convertToFloat() == 0.f) && (BO->getOpcode() == Instruction::FAdd) ||
                    (RHSC->getValue().convertToFloat() == 1.f) && (BO->getOpcode() == Instruction::FMul))
                {
                    LO_vec.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
            }

            else if (auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1))) {
                if (RHSC->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd ||
                    RHSC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul)
                {
                    RO_vec.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
            }
        }
        else if (auto CI = dyn_cast<CallInst>(&I)) {
            if (CI->getCalledFunction()->getName() == StringRef("powf")) {
                if (auto instr = dyn_cast<ConstantFP>(CI->getArgOperand(1))){
                    if (instr->getValue().convertToFloat() == 2.f){
                        IC_vec.push_back(CI);
                        pa = PreservedAnalyses::none();
                    }
                }
            }
        }
    }
    while (!RO_vec.empty()) {
        auto BO = RO_vec.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(0));
        BO->eraseFromParent();
    }
    while (!LO_vec.empty()) {
        auto BO = LO_vec.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(1));
        BO->eraseFromParent();
    }
    while (!IC_vec.empty()) {
        auto CI = IC_vec.pop_back_val();
        auto operand = CI->getOperand(0);
        auto itr = BinaryOperator::Create(Instruction::FMul, operand, operand);
        CI->replaceAllUsesWith(itr);
        itr->insertBefore(CI);
        CI->eraseFromParent();
    }
    return pa;
}