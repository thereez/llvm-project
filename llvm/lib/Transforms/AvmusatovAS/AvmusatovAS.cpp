#include "llvm/Transforms/AvmusatovAS/AvmusatovAS.h"

using namespace llvm;

void simplifyBinaryOperators(SmallVector<BinaryOperator*, 16>& binOperators, char position) {
    while (!binOperators.empty()) {
        auto binOperator = binOperators.pop_back_val();
        binOperator->replaceAllUsesWith(binOperator->getOperand(position));
        binOperator->eraseFromParent();
    }
}

void replacePowerInstrucions(SmallVector<CallInst*, 16>& powerInstructions) {
    while (!powerInstructions.empty()) {
        auto powerInstruction = powerInstructions.pop_back_val();
        auto value = powerInstruction->getArgOperand(LEFT_OP);
        auto mulOperator = BinaryOperator::Create(Instruction::FMul, value, value);
        ReplaceInstWithInst(powerInstruction, mulOperator);
    }
}

PreservedAnalyses AvmusatovAS::run(Function& F, FunctionAnalysisManager& AM) {
    auto pa = PreservedAnalyses::all();

    SmallVector<CallInst*, 16> powerWarkSet;
    SmallVector<BinaryOperator*, 16> leftWarkSet;
    SmallVector<BinaryOperator*, 16> rightWarkSet;

    for (auto& I : instructions(F)) {
        if (auto BO = dyn_cast<BinaryOperator>(&I)) {
            auto opCode = BO->getOpcode();

            if (auto leftOf = dyn_cast<ConstantFP>(BO->getOperand(LEFT_OP))) {
                auto leftOpValue = leftOf->getValue().convertToFloat();
                if (((leftOpValue == 0.f) && (opCode == Instruction::FAdd)) || ((leftOpValue == 1.f) && (opCode == Instruction::FMul))) {
                    leftWarkSet.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
            }

            if (auto rightOp = dyn_cast<ConstantFP>(BO->getOperand(RIGHT_OP))) {
                auto rightOpValue = rightOp->getValue().convertToFloat();
                if (((rightOpValue == 0.f) && (opCode == Instruction::FAdd)) || ((rightOpValue == 1.f) && (opCode == Instruction::FMul))) {
                    rightWarkSet.push_back(BO);
                    pa = PreservedAnalyses::none();
                }
            }
        }

        if (auto CI = dyn_cast<CallInst>(&I)) {
            if (CI->getCalledFunction()->getName() == "powf") {
                if (dyn_cast<ConstantFP>(CI->getArgOperand(RIGHT_OP))->getValue().convertToFloat() == 2.f) {
                    powerWarkSet.push_back(CI);
                    pa = PreservedAnalyses::none();
                }
            }
        }
    }

    simplifyBinaryOperators(leftWarkSet, RIGHT_OP);
    simplifyBinaryOperators(rightWarkSet, LEFT_OP);
    replacePowerInstrucions(powerWarkSet);

    return pa;
}