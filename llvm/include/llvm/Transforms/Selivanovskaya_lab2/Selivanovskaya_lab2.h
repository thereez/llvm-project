#pragma once

#define DEBUG_TYPE "Selivanovskaya_lab2"

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/IntrinsicInst.h"


namespace llvm {


class Selivanovskaya_lab2 : public PassInfoMixin<Selivanovskaya_lab2> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  static bool isRequired() { return true; }
};


}
#undef DEBUG_TYPE
