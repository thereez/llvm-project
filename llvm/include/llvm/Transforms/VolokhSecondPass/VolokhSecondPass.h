#pragma once

#define DEBUG_TYPE "volokhpass"

#include "llvm/IR/PassManager.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/SmallVector.h"


namespace llvm {
    class VolokhSecondPass : public PassInfoMixin<VolokhPass> {
        public:
            PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
            static bool isRequired() {return true;}
    };

}