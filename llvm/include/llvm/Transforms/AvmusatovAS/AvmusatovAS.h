#ifndef LLVM_TRANSFORMS_AVMUSATOVAS_AVMUSATOVAS_H
#define LLVM_TRANSFORMS_AVMUSATOVAS_AVMUSATOVAS_H

#define LEFT_OP 0
#define RIGHT_OP 1

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

namespace llvm {

  class AvmusatovAS : public PassInfoMixin<AvmusatovAS> {
  public:
    PreservedAnalyses run(Function& F, FunctionAnalysisManager& AM);
    static bool isRequired() { return true; }
  };
} // namespace llvm

#endif //LLVM_TRANSFORMS_AVMUSATOVAS_AVMUSATOVAS_H