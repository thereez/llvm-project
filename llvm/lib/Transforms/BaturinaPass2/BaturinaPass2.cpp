#include "llvm/Transforms/BaturinaPass2/BaturinaPass2.h"

using namespace llvm;

auto pa = PreservedAnalyses::all();

    SmallVector<BinaryOperator*, 16> var_right;
    SmallVector<BinaryOperator*, 16> var_left;
    SmallVector<CallInst*, 16> pow;

    for (auto& instruction : instructions(F)) {
        if (auto bin_op = dyn_cast<BinaryOperator>(&instruction)){
            if(auto r_x = dyn_cast<ConstantFP>(bin_op->getOperand(1))) {
                if(r_x->getValue().convertToFloat() == 0.f && bin_op->getOpcode() == Instruction::FAdd) {
                    var_right.push_back(bin_op);
                    pa = PreservedAnalyses::none();
                } if(r_x->getValue().convertToFloat() == 1.f && bin_op->getOpcode() == Instruction::FMul) {
                    var_right.push_back(bin_op);
                    pa = PreservedAnalyses::none();
                }
            }
            else if(auto l_x = dyn_cast<ConstantFP>(bin_op->getOperand(0))) {
                if(l_x->getValue().convertToFloat() == 0.f && bin_op->getOpcode() == Instruction::FAdd) {
                    var_left.push_back(bin_op);
                    pa = PreservedAnalyses::none();
                } if(l_x->getValue().convertToFloat() == 1.f && bin_op->getOpcode() == Instruction::FMul) {
                    var_left.push_back(bin_op);
                    pa = PreservedAnalyses::none();
                } 
            }
        }
        if (auto call_inst = dyn_cast<CallInst>(&instruction)){
          if(call_inst->getCalledFunction()->getName() == "powf"){
            if(auto arg = dyn_cast<ConstantFP>(call_inst->getOperand(1))){
              if (arg->getValue().convertToFloat() == 2.f){
                pow.push_back(call_inst);
                pa = PreservedAnalyses::none();
              }
            }
          }
        }
    }
    while (!var_right.empty()) {
        auto bin_op = var_right.pop_back_val();
        bin_op->replaceAllUsesWith(bin_op->getOperand(0));
        bin_op->eraseFromParent();
    } 
    while (!var_left.empty()) {
        auto bin_op = var_left.pop_back_val();
        bin_op->replaceAllUsesWith(bin_op->getOperand(1));
        bin_op->eraseFromParent();
    } 
    while (!pow.empty()) {
        auto call_inst = pow.pop_back_val();
        auto x = call_inst->getOperand(0);
        static LLVMContext context;
        IRBuilder<> Builder(context);
        auto mul = dyn_cast<BinaryOperator>(Builder.CreateFMul(x, x));
        call_inst->replaceAllUsesWith(mul);
        mul->insertBefore(call_inst);
        call_inst->eraseFromParent();
    }  
    return pa;
}