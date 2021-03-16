#include "llvm/Transforms/AKomyaginASPass/AKomyaginASPass.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"


using namespace llvm;

PreservedAnalyses AKomyaginASPass::run(Function &F, FunctionAnalysisManager &AM)
{
    auto pa = PreservedAnalyses::all();

    SmallVector<BinaryOperator*, 16> wark_set0;
    SmallVector<BinaryOperator*, 16> wark_set1;
    SmallVector<CallBase*, 16> pows;

    for(auto& I : instructions(F)){
        if(auto BO = dyn_cast<BinaryOperator>(&I)){
            auto RHSC0 = dyn_cast<ConstantFP>(BO->getOperand(0));
            auto RHSC1 = dyn_cast<ConstantFP>(BO->getOperand(1));
            if(RHSC0 || RHSC1){
                if(RHSC0){
                    if(RHSC0->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) {
                        wark_set0.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                    else if(RHSC0 && RHSC0->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) {
                        wark_set0.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                }
                else if(RHSC1){
                    if(RHSC1->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul) {
                        wark_set1.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                    else if(RHSC1 && RHSC1->getValue().convertToFloat() == 0.f && BO->getOpcode() == Instruction::FAdd) {
                        wark_set1.push_back(BO);
                        pa = PreservedAnalyses::none();
                    }
                }
            }
        }

        if(auto pow = dyn_cast<CallBase>(&I)){
            if(pow->getCalledFunction()->getName()=="powf"){
                if(auto RHSC = dyn_cast<ConstantFP>(pow->getArgOperand(1))){ //2.f
                    if(RHSC->getValue().convertToFloat()==2.f){
                        pows.push_back(pow);
                        pa = PreservedAnalyses::none();
                    }
                }
            }
        }
    }
    
    while(!wark_set0.empty()){
        auto BO = wark_set0.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(1));
        BO->eraseFromParent();
    }

    while(!wark_set1.empty()){
        auto BO = wark_set1.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(0));
        BO->eraseFromParent();
    }

    while(!pows.empty()){
        auto CB = pows.pop_back_val();
        auto operand = CB->getArgOperand(0);
        auto BO = BinaryOperator::Create(Instruction::FMul, operand, operand);
        CB->replaceAllUsesWith(BO);
        BO->insertAfter(CB);
        CB->eraseFromParent();
    }

    return pa;
}




