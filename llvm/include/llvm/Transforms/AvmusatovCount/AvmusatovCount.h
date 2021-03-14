//===-- HelloWorld.h - Example Transformations ------------------*- C++ -*-===//
//
//  The pass counting only function definitions, loops, Basic blocks, Arithmetic instructions of type add and mull.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_AVMUSATOVCOUNT_AVMUSATOVCOUNT_H
#define LLVM_TRANSFORMS_AVMUSATOVCOUNT_AVMUSATOVCOUNT_H

#define DEBUG_TYPE "AvmusatovCount"

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"

STATISTIC(funcDefCount, "Number of function definitions");
STATISTIC(loopsCount, "Number of loops");
STATISTIC(basicBlocksCount, "Number of basic blocks");
STATISTIC(mulOperationCount, "Number of mul operations");
STATISTIC(addOperationCount, "Number of add operations");

namespace llvm {

  class AvmusatovCount : public PassInfoMixin<AvmusatovCount> {
  public:
    PreservedAnalyses run(Function& F, FunctionAnalysisManager& AM);
    static bool isRequired() { return true; }
  };
} // namespace llvm
#undef DEBUG_TYPE
#endif //LLVM_TRANSFORMS_AVMUSATOVCOUNT_AVMUSATOVCOUNT_H