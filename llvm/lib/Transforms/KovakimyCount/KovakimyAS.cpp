#include "llvm/Transforms/KovakimyCount/KovakimyAS.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#define DEBUG_TYPE "kovakimypass"

#include "llvm/IR/InstIterator.h"
#include "llvm/ADT/SmallVector.h"

using namespace llvm;

PreservedAnalyses KovakimyPass::run(Function &F, FunctionAnalysisManager &AM) 
{ 
     auto pa = PreservedAnalyses::all();
     SmallVector<BinaryOperator *, 16> wark_set_l;
     SmallVector<BinaryOperator *, 16> wark_set_r;
     SmallVector<CallBase *, 16> wark_set_pow;
     
     for (Instruction &I : instructions(F)) {
       if (BinaryOperator *BO = dyn_cast<BinaryOperator>(&I)) {
         if (ConstantFP *RHSC = dyn_cast<ConstantFP>(BO->getOperand(1))) {
           if (RHSC->getValue().convertToFloat() == 0.f &&
               BO->getOpcode() == Instruction::FAdd) {
             wark_set_r.push_back(BO);
             pa = PreservedAnalyses::none();
           }
           if (RHSC->getValue().convertToFloat() == 1.f &&
               BO->getOpcode() == Instruction::FMul) {
             wark_set_r.push_back(BO);
             pa = PreservedAnalyses::none();
           }
         } else if (ConstantFP *RHSC =
                        dyn_cast<ConstantFP>(BO->getOperand(0))) {
           if (RHSC->getValue().convertToFloat() == 0.f &&
               BO->getOpcode() == Instruction::FAdd) {
             wark_set_l.push_back(BO);
             pa = PreservedAnalyses::none();
           }
           if (RHSC->getValue().convertToFloat() == 1.f &&
               BO->getOpcode() == Instruction::FMul) {
             wark_set_l.push_back(BO);
             pa = PreservedAnalyses::none();
           }
         }
       }
       if (CallInst *C = dyn_cast<CallInst>(&I)) {
         if (C->getCalledFunction()->getName() == "powf") {
           if (ConstantFP *FPV = dyn_cast<ConstantFP>(C->getOperand(1))) {
             if (FPV->getValue().convertToFloat() == 2.f) {
               wark_set_pow.push_back(C);
               pa = PreservedAnalyses::none();
             }
           }
         }
       }
     }
     while (!wark_set_r.empty()) 
      {
       BinaryOperator *BO = wark_set_r.pop_back_val();
       BO->replaceAllUsesWith(BO->getOperand(0));
       BO->eraseFromParent();
      }

      while (!wark_set_l.empty()) 
      {
        BinaryOperator *BO = wark_set_l.pop_back_val();
        BO->replaceAllUsesWith(BO->getOperand(1));
        BO->eraseFromParent();
      }

      while (!wark_set_pow.empty()) 
      {
        CallBase *CB = wark_set_pow.pop_back_val();
        BinaryOperator* BO_MUL = BinaryOperator::Create(Instruction::FMul, CB->getOperand(0), CB->getOperand(0));
        CB->replaceAllUsesWith(BO_MUL);
        BO_MUL->insertBefore(CB);
        CB->eraseFromParent();
      }
     return pa;
}