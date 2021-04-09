#include "llvm/Analysis/IDoroshenkoAnalysis.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"

namespace llvm {

AnalysisKey IDoroshenkoAnalysis::Key;

IDoroshenkoAnalysis::Result IDoroshenkoAnalysis::run(Function& F, FunctionAnalysisManager & FAM) {
    Result result;

    for (Function::iterator basicBlock = F.begin(); basicBlock != F.end(); ++basicBlock) {
        for (BasicBlock::iterator instruction = basicBlock->begin() ; instruction != basicBlock->end(); ++instruction) {
            if (std::string(instruction->getOpcodeName()) == "add" || std::string(instruction->getOpcodeName()) == "fadd")
                result.TotalAdd++;
            if (std::string(instruction->getOpcodeName()) == "mul" || std::string(instruction->getOpcodeName()) == "fmul")
                result.TotalMul++;
        }
    }

    for (auto& I : instructions(F)) {
        if (auto C = dyn_cast<CallInst>(&I)) 
            if(C->getCalledFunction()->getName() == "powf") 
                result.TotalPow++;
    }
    return result;
}

void IDoroshenkoAnalysisInfo::print(raw_ostream &OS) const {
    OS << "Total pow: " << TotalPow << "\n";
    OS << "Total add: " << TotalAdd << "\n";
    OS << "Total mul: " << TotalMul << "\n";
}

}