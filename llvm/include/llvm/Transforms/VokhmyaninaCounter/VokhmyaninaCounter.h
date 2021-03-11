#pragma once

#define DEBUG_TYPE "VokhmyaninaCounter"
#include "llvm/ADT/Statistic.h"

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"


namespace llvm {

class VokhmyaninaCounter : public PassInfoMixin<VokhmyaninaCounter> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired() { return true; }
};

} // namespace llvm
