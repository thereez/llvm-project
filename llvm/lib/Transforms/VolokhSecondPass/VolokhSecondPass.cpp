#include "llvm/include/llvm/Transforms/VolokhSecondPass/VolokhSecondPass.h"

using namespace llvm;

PreservedAnalyses VolokhSecondPass::run(Function &func, FunctionAnalysisManager &AM){
    auto pa = PreservedAnalyses::all();

    SmallVector<BinaryOperator*, 16> wark_set_lefts; // left elem 0 (add) or 1 (mul)
    SmallVector<BinaryOperator*, 16> wark_set_rights; // right elem 0 or 1
    SmallVector<CallInst*, 16> powfs; // x^2 = x*x

    for (auto& instr : instructions(func)) {
        // binary operators
        if (auto binaryOp = dyn_cast<BinaryOperator>(&instr)){
            // left elem
            if(auto left = dyn_cast<ConstantFP>(binaryOp->getOperand(0))) {
                if(left->getValue().convertToFloat() == 0.f) {
                    if (binaryOp->getOpcode() == Instruction::FAdd){
                        wark_set_lefts.push_back(binaryOp);
                        pa = PreservedAnalyses::none();
                    }
                }
                if(left->getValue().convertToFloat() == 1.f) {
                    if(binaryOp->getOpcode() == Instruction::FMul){
                        wark_set_lefts.push_back(binaryOp);
                        pa = PreservedAnalyses::none();
                    }
                } 
            }
            // right elem
            else if(auto right = dyn_cast<ConstantFP>(binaryOp->getOperand(1))) {
                if(right->getValue().convertToFloat() == 0.f) {
                    if (binaryOp->getOpcode() == Instruction::FAdd){
                        wark_set_rights.push_back(binaryOp);
                        pa = PreservedAnalyses::none();
                    }
                }
                if(right->getValue().convertToFloat() == 1.f) {
                    if (binaryOp->getOpcode() == Instruction::FMul){
                        wark_set_rights.push_back(binaryOp);
                        pa = PreservedAnalyses::none();
                    }
                }

            }
        }

        // func
        if (auto callInst = dyn_cast<CallInst>(&instr)){
            if (callInst->getCalledFunction()->getName() == "powf"){
                if (auto arg = dyn_cast<ConstantFP>(callInst->getOperand(1))){
                    if (arg->getValue().convertToFloat() == 2.f){
                        powfs.push_back(callInst);
                        pa = PreservedAnalyses::none();
                    }
                }
            }
        }

    }
    
    // del left
    while (!wark_set_lefts.empty()) {
        auto binaryOp = wark_set_lefts.pop_back_val();
        binaryOp->replaceAllUsesWith(binaryOp->getOperand(1));
        binaryOp->eraseFromParent();
    }

    // del right
    while (!wark_set_rights.empty()) {
        auto binaryOp = wark_set_rights.pop_back_val();
        binaryOp->replaceAllUsesWith(binaryOp->getOperand(0));
        binaryOp->eraseFromParent();
    }

    // replace pow
    while (!powfs.empty()) {
        auto callInst = powfs.pop_back_val();
        auto mul = BinaryOperator::Create(Instruction::FMul, callInst->getOperand(0), callInst->getOperand(0));
        callInst->replaceAllUsesWith(mul);
        mul->insertBefore(callInst);
        callInst->eraseFromParent();
    }
    
    return pa;

}