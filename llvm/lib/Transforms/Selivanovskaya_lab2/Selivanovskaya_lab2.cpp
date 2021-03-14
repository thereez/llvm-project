#include "llvm/Transforms/Selivanovskaya_lab2/Selivanovskaya_lab2.h"

using namespace llvm;
using namespace std;


PreservedAnalyses Selivanovskaya_lab2::run(Function &F, FunctionAnalysisManager &AM) {
    auto pa = PreservedAnalyses::all();
 
    SmallVector<CallInst*, 16> pows_wset;
    SmallVector<BinaryOperator*, 16> right_wset;
    SmallVector<BinaryOperator*, 16> left_wset;
    
    for (auto& I : instructions(F)) { 

        if (auto CInst = dyn_cast<CallInst>(&I)){
            if (CInst->getCalledFunction()->getName() == "powf"){
                if (auto argv = dyn_cast<ConstantFP>(CInst->getOperand(1))){
                    if (argv->getValue().convertToFloat() == 2.f){
                        pa = PreservedAnalyses::none();
                        pows_wset.push_back(CInst);
                    }
                }
            }
        }

        if (auto BinOp = dyn_cast<BinaryOperator>(&I)){
            if (auto RHSC_l = dyn_cast<ConstantFP>(BinOp->getOperand(0))) {

                if (RHSC_l->getValue().convertToFloat() == 1.f && BinOp->getOpcode() == Instruction::FMul) {
                    pa = PreservedAnalyses::none();
                    left_wset.push_back(BinOp);
                }
                if (RHSC_l->getValue().convertToFloat() == 0.f && BinOp->getOpcode() == Instruction::FAdd) {
                    pa = PreservedAnalyses::none();
                    left_wset.push_back(BinOp);
                } 

            }
            else if (auto RHSC_r = dyn_cast<ConstantFP>(BinOp->getOperand(1))) {

                if (RHSC_r->getValue().convertToFloat() == 1.f && BinOp->getOpcode() == Instruction::FMul) {
                    pa = PreservedAnalyses::none();
                    right_wset.push_back(BinOp);
                }
                if (RHSC_r->getValue().convertToFloat() == 0.f && BinOp->getOpcode() == Instruction::FAdd) {
                    pa = PreservedAnalyses::none();
                    right_wset.push_back(BinOp);
                }  
            }
        }

    }

    while (!pows_wset.empty()) {
        auto CInst = pows_wset.pop_back_val();
        auto x = BinaryOperator::Create(Instruction::FMul, CInst->getOperand(0), CInst->getOperand(0));
        CInst->replaceAllUsesWith(x);
        x->insertBefore(CInst);
        CInst->eraseFromParent();
    }

    while (!left_wset.empty()) {
        auto BinOp = left_wset.pop_back_val();
        BinOp->replaceAllUsesWith(BinOp->getOperand(1));
        BinOp->eraseFromParent();
    }

    while (!right_wset.empty()) {
        auto BinOp = right_wset.pop_back_val();
        BinOp->replaceAllUsesWith(BinOp->getOperand(0));
        BinOp->eraseFromParent();
    }

    return pa;
}


