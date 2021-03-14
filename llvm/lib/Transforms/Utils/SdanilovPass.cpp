#include "llvm/Transforms/Utils/SdanilovPass.h"
#include <iostream>
using namespace llvm;

void SdanilovPass::countLoopsNested(Loop* loop){
  std::vector<Loop*> subLoop = loop->getSubLoops();
  LoopCounter+= subLoop.size();
}

PreservedAnalyses SdanilovPass::run(Function &F, FunctionAnalysisManager &AM){
  auto pa = PreservedAnalyses::all();
  SmallVector<BinaryOperator*, 16> weak_ops;
  SmallVector<BinaryOperator*, 16> pow2_ops;
  SmallVector<CallBase*, 16> pow2_calls;

  for (auto &BB: F){
    for (auto &I : BB){
      if (auto BO = dyn_cast<BinaryOperator>(&I)){
        if (auto OPC = dyn_cast<ConstantFP>(BO->getOperand(1))){
          if (OPC->getValue().convertToFloat() == 1.f && BO->getOpcode() == Instruction::FMul){
            weak_ops.push_back(BO);
            pa = PreservedAnalyses::none();
            errs() << "y = x * 1 => y = x \n";
          }
        }
      }
      if (auto POW = dyn_cast<CallBase>(&I)){
        Function* FCalled = POW->getCalledFunction();
        errs() << FCalled->getName() << " ";
        if ((std::string)FCalled->getName() == "llvm.pow.f32"){
          errs() << "enter in powf function \n";
          Value* X;
          int num = 0;
          for (auto arg = POW->arg_begin(); arg != POW->arg_end(); ++arg){
            if (num == 0){
              X = (Value*)arg;
            }
            if (num==1){
              if (auto step = dyn_cast<ConstantFP>(arg)){
                errs() << "stepen is " << step->getValue().convertToFloat() << "\n";
                if (step->getValue().convertToFloat() == 2.f){
                  BinaryOperator* BO = BinaryOperator::Create(Instruction::FMul, X, X);
                  pow2_ops.push_back(BO);
                  pow2_calls.push_back(POW);
                  errs() << "y = powf(x, 2.f) => y = x*x \n";
                  pa = PreservedAnalyses::none();
                }
              }
            }
            num++;
          }
        }
      }
    }
  }

  while(!weak_ops.empty()){
    auto BO = weak_ops.pop_back_val();
    BO->replaceAllUsesWith(BO->getOperand(0));
    BO->eraseFromParent();
  }

  while(!pow2_ops.empty()){
    auto POW2 = pow2_ops.pop_back_val();
    auto MUL = pow2_calls.pop_back_val();
    POW2->replaceAllUsesWith(MUL);
    POW2->eraseFromParent();
  }

  //Lab1 Count Loops, Functions, BBs
  LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
  if (!F.isDeclaration()){
    ++FunctionCounter;
  }
  for (auto &BB: F){
    BasicBlocksCounter++;
    for (auto &I : BB){
      std::string opcodename = I.getOpcodeName();
      if (opcodename == "fadd" || opcodename == "fmul" || opcodename == "mul" || opcodename == "add"){
          AddMulCounter++;
      }
    }
  }
  for (LoopInfo::iterator i = LI.begin(), b = LI.end(); i!=b; ++i){
    LoopCounter++;
    Loop *loopPtr = *i;
    countLoopsNested(loopPtr);
  }
  return pa;
};



