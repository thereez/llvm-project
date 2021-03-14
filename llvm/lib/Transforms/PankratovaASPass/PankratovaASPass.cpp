#include "llvm/Transforms/PankratovaASPass/PankratovaASPass.h"

using namespace llvm;


bool makeAlgebraicSimplification(Instruction &I) {
    if (I.isBinaryOp()) {
        auto opcode = I.getOpcode();
        auto firstOperand = dyn_cast<ConstantFP>(I.getOperand(0));
        if (firstOperand &&
           ((opcode == Instruction::FAdd && firstOperand->getValue().convertToFloat() == 0.f) ||
            (opcode == Instruction::FMul && firstOperand->getValue().convertToFloat() == 1.f))) {
            I.replaceAllUsesWith(I.getOperand(1));
            return true;
        }
        auto secondOperand = dyn_cast<ConstantFP>(I.getOperand(1));
        if (secondOperand &&
           ((opcode == Instruction::FAdd && secondOperand->getValue().convertToFloat() == 0.f) ||
            (opcode == Instruction::FMul && secondOperand->getValue().convertToFloat() == 1.f))) {
            I.replaceAllUsesWith(I.getOperand(0));
            return true;
        }
    }
    return false;
}

bool makeStrengthReduction(Instruction &I) {
    auto pow = dyn_cast<CallBase>(&I);
    if (pow && pow->getCalledFunction()->getName() == "powf") {
        auto secondOperand = dyn_cast<ConstantFP>(pow->getArgOperand(1));
        if (secondOperand && secondOperand->getValue().convertToFloat() == 2.f) {
            auto firstOperand = pow->getArgOperand(0);
            auto newInstr = BinaryOperator::Create(Instruction::FMul, firstOperand, firstOperand);
            I.replaceAllUsesWith(newInstr);
            newInstr->insertAfter(&I);
            return true;
        }
    }
    return false;
}

PreservedAnalyses PankratovaASPass::run(Function &F, FunctionAnalysisManager &AM) {
    auto PA = PreservedAnalyses::all();
    for (BasicBlock &BB : F) {
        auto instrIter = BB.begin();
        while (instrIter != BB.end()) {
            if (makeAlgebraicSimplification(*instrIter) || makeStrengthReduction(*instrIter)) {
                instrIter = instrIter->eraseFromParent();
                PA = PreservedAnalyses::none();
            }
            else {
                instrIter++;
            }
        }
    }
    return PA;
}
