#pragma once

#define DEBUG_TYPE "VokhmyaninaLab2"
#include "llvm/ADT/Statistic.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"

#include "llvm/IR/PassManager.h"


namespace llvm {

class VokhmyaninaLab2 : public PassInfoMixin<VokhmyaninaLab2> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired() { return true; }
};

} // namespace llvm
