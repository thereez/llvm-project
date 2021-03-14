
#include "llvm/Transforms/Korkunov_ASpass/Korkunov_ASpass.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"

#define DEBUG_TYPE "Korkunov_ASpass"
#include "llvm/ADT/Statistic.h"

using namespace llvm;

PreservedAnalyses Korkunov_ASpass::run(Function &F, FunctionAnalysisManager &AM){
    auto pa = PreservedAnalyses::all();
   // CallInst ci = CallInst::Create(F);
    SmallVector<BinaryOperator*, 16> wark_set;
    SmallVector<BinaryOperator*, 16> wark_set_left;
    SmallVector<CallBase*, 16> powfs;
    for(auto& I : instructions(F)){

        if(auto ci = dyn_cast<CallBase>(&I)){
            if(ci->getCalledFunction()->getName()=="llvm.pow.f32"){
                if(auto val = dyn_cast<ConstantFP>(ci->getArgOperand(1)))
                    if(val->getValue().convertToFloat()==2.f){
                        powfs.push_back(ci);
                        pa = PreservedAnalyses::none();
                    }
                      //  errs()<<val->getValue().convertToFloat()<< "\n";     
            }
        }

        if(auto BO = dyn_cast<BinaryOperator>(&I)){
             auto RHSC = dyn_cast<ConstantFP>(BO->getOperand(1));
             auto RHSC_left = dyn_cast<ConstantFP>(BO->getOperand(0));
            if(BO->getOpcode() == Instruction::FAdd){
                if(RHSC || RHSC_left){
                    if(RHSC && RHSC->getValue().convertToFloat() == 0.f) {
                        wark_set.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                    else if(RHSC_left && RHSC_left->getValue().convertToFloat() == 0.f) {
                        wark_set_left.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                }
            }
            else if (BO->getOpcode() == Instruction::FMul){
                if(RHSC || RHSC_left){
                    if(RHSC && RHSC->getValue().convertToFloat() == 1.f) {
                        wark_set.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                    else if(RHSC_left && RHSC_left->getValue().convertToFloat() == 1.f) {
                        wark_set_left.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                }
            }
        }
    }
    while (!wark_set_left.empty()){
        auto BO = wark_set_left.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(1));
        BO->eraseFromParent();
    }
    while (!wark_set.empty()){
        auto BO = wark_set.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(0));
        BO->eraseFromParent();
    }
   // LLVMContext context;
  //  IRBuilder<> irBuilder(context);
    while (!powfs.empty()){
        auto CB = powfs.pop_back_val(); 
   //     Value* tmp = irBuilder.CreateFMul(x, y);
        //auto ttmp = (Value*)CB->getArgOperand(0);
       // auto ttmp = CB->getOperand(0);
     //   auto tmp = irBuilder.CreateFMul(CB->getArgOperand(0), CB->getArgOperand(0));
        auto tmp = BinaryOperator::Create(Instruction::FMul, CB->getArgOperand(0), CB->getArgOperand(0));
        tmp->replaceAllUsesWith(CB);
        CB->replaceAllUsesWith(tmp);
        tmp->insertAfter(CB);
        CB->eraseFromParent();
    }
    return pa;
}//call inst