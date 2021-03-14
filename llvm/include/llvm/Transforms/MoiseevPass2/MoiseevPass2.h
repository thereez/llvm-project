#pragma once

#define DEBUG_TYPE "MoiseevPass2"

#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IRBuilder.h"


namespace llvm {
    class MoiseevPass2 : public PassInfoMixin<MoiseevPass2> {
        public:
            PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
            static bool isRequired() {return true;}
    };
} 