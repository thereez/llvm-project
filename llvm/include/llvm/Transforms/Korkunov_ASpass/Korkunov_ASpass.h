
#ifndef LLVVM_TRANSFORMS_KORKUNOVASPASS_KORKUNOVASPASS_H
#define LLVVM_TRANSFORMS_KORKUNOVASPASS_KORKUNOVASPASS_H

#include "llvm/IR/PassManager.h"

namespace llvm{

    class LPMUpdater;

    class Korkunov_ASpass : public PassInfoMixin<Korkunov_ASpass>{
        public:
            PreservedAnalyses run(Function& F, FunctionAnalysisManager& AM);
    };

}

#endif