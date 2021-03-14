#ifndef LLVM_TRANSFORMS_SDANILOV_COUNTING_PASS
#define LLVM_TRANSFORMS_SDANILOV_COUNTING_PASS

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"

#define DEBUG_TYPE "task"
STATISTIC(FunctionCounter, "Counts number of declarations of functions");
STATISTIC(BasicBlocksCounter, "Counts number of BB");
STATISTIC(AddMulCounter, "Counts number of + and * operations");
STATISTIC(LoopCounter, "Counts number of loops");

namespace llvm {
    class SdanilovPass : public PassInfoMixin<SdanilovPass> {
    public:
        void countLoopsNested(Loop* loop);
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
        static bool isRequired(){return true;};
    };
}
#undef DEBUG_TYPE
#endif
