#pragma once

#define DEBUG_TYPE "Selivanovskaya_lab1"

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/ADT/Statistic.h"

namespace llvm {

class Selivanovskaya_lab1 : public PassInfoMixin<Selivanovskaya_lab1> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired() { return true; }
};

} // namespace llvm