#include "llvm/Transforms/Sazanov_Lab2_Pass/Sazanov_Lab2_Pass.h"

using namespace llvm;

#define DEBUG_TYPE "Sazanov_Lab1_Pass"

const int LEFT = 0;
const int RIGHT = 1;

void simplify(SmallVector<BinaryOperator*, 32> &ops, int side)
{
	for (auto op : ops)
	{
		op->replaceAllUsesWith(op->getOperand(side));
		op->eraseFromParent();
	}
	ops.clear();
}

PreservedAnalyses Sazanov_Lab2_Pass::run(Function &F, FunctionAnalysisManager &AM)
{
	auto analyses = PreservedAnalyses::all();

	SmallVector<BinaryOperator*, 32> lefts;
	SmallVector<BinaryOperator*, 32> rights;
	SmallVector<CallInst*, 32> pows;

	for (auto &I : instructions(F))
	{
		if (auto binary_operator = dyn_cast<BinaryOperator>(&I))
		{
			auto leftValue = dyn_cast<ConstantFP>(binary_operator->getOperand(LEFT))->getValue().convertToFloat();
            auto rightValue = dyn_cast<ConstantFP>(binary_operator->getOperand(RIGHT))->getValue().convertToFloat();
            auto opcode = binary_operator->getOpcode();

			if (opcode == Instruction::FMul)
			{
				if (leftValue == 1.f)
					lefts.push_back(binary_operator);
				else if (rightValue == 1.f)
					rights.push_back(binary_operator);
			}
			else if (opcode == Instruction::FAdd)
			{
				if (leftValue == 0.f)
					lefts.push_back(binary_operator);
				else if (rightValue == 0.f)
					rights.push_back(binary_operator);
			}
		}

		if (auto call_inst = dyn_cast<CallInst>(&I))
		{
			if ((call_inst->getCalledFunction()->getName() == "powf") &&
				(dyn_cast<ConstantFP>(call_inst->getArgOperand(RIGHT))->getValue().convertToFloat() == 2.f))
			{
				pows.push_back(call_inst);
			}
		}
	}

	if (!lefts.empty() || !rights.empty() || !pows.empty())
	{
		analyses = PreservedAnalyses::none();
	}

	simplify(lefts, RIGHT);
	simplify(rights, LEFT);

	for (auto pow : pows)
	{
		auto op = pow->getArgOperand(LEFT);
		auto new_op = BinaryOperator::Create(Instruction::FMul, op, op);
		pow->replaceAllUsesWith(new_op);
		pow->eraseFromParent();
	}
	pows.clear();

	return analyses;
}