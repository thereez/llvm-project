#include "llvm/Transforms/Pyanzin_ASPass/Pyanzin_ASPass.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/IRBuilder.h"

#define DEBUG_TYPE "pyanzin_aspass"

using namespace llvm;

PreservedAnalyses Pyanzin_ASPass::run(Function &F,
                                      FunctionAnalysisManager &AM) 
                                    {
    auto pa = PreservedAnalyses::all();
    SmallVector<BinaryOperator*, 16> wark_set_val0;
    SmallVector<BinaryOperator*, 16> wark_set_val1;
    SmallVector<CallInst *, 16> wark_set_powf;
    for (auto& I : instructions(F)) {

        if (auto CI = dyn_cast<CallInst>(&I))
        {
            if (CI->getCalledFunction()->getName() == "powf" ) 
            {
                auto RHSC = dyn_cast<ConstantFP>(CI->getOperand(1));
                if (RHSC->getValue().convertToFloat() == 2.0f)
                {
                    errs() << "found function to optimize" << "\n";
                    wark_set_powf.push_back(CI);
                    pa = PreservedAnalyses::none();
                }
            }
        }

        if (auto BO = dyn_cast<BinaryOperator>(&I)) {
            auto RHSC_0 = dyn_cast<ConstantFP>(BO->getOperand(0));
            auto RHSC_1 = dyn_cast<ConstantFP>(BO->getOperand(1));
            if (BO->getOpcode() == Instruction::FAdd) {
                if (RHSC_0) {
                    if (RHSC_0->getValue().convertToFloat() == 0.f) {
                        wark_set_val0.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                }
                if (RHSC_1) {
                    if (RHSC_1->getValue().convertToFloat() == 0.f) {
                        wark_set_val1.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                }
            }

            if (BO->getOpcode() == Instruction::FMul) {
                if (RHSC_0) {
                    if (RHSC_0->getValue().convertToFloat() == 1.f) {
                        wark_set_val0.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                }
                if (RHSC_1) {
                    if (RHSC_1->getValue().convertToFloat() == 1.f) {
                        wark_set_val1.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                }
            }
        }
    }

    while(!wark_set_val0.empty()) {
        auto BO = wark_set_val0.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(1));
        BO->eraseFromParent();
    }

    while(!wark_set_val1.empty()) {
        auto BO = wark_set_val1.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(0));
        BO->eraseFromParent();
    }

    while(!wark_set_powf.empty()) {
        auto CI = wark_set_powf.pop_back_val();
        IRBuilder<> builder(CI);
        Value* mul = builder.CreateFMul(CI->getOperand(0),CI->getOperand(0));
        for (auto &U : CI->uses())
        {
            User* user = U.getUser();
            user->setOperand(U.getOperandNo(), mul);
            CI->eraseFromParent();
        }
    }

    return pa;
}