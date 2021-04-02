#include "llvm/Analysis/PyanzinAnalysis.h"
#include "llvm/IR/Instructions.h"

using namespace llvm;





PyanzinAnalysisInfo
PyanzinAnalysisInfo::getPyanzinAnalysisInfo(const Function &F,
                                                  const LoopInfo &LI) {

    PyanzinAnalysisInfo FPI;

    for(const BasicBlock &BB : F)
    {
        FPI.bb_count++;
        for(const Instruction &I: BB)
        {
            std::string inst = std::string(I.getOpcodeName());
            if (inst == "add" || inst == "fadd") FPI.add_count++;
            if (inst == "mul" || inst == "fmul") FPI.mul_count++; 
        }
    }
  return FPI;
}

void PyanzinAnalysisInfo::print(raw_ostream &OS) const {
  OS << "add instructions: " << add_count << "\n"
     << "mul instrictions: " << mul_count << "\n"
     << "basic blocks: " << bb_count << "\n\n";
    
}

AnalysisKey PyanzinAnalysis::Key;

PyanzinAnalysisInfo
PyanzinAnalysis::run(Function &F, FunctionAnalysisManager &FAM) {
  return PyanzinAnalysisInfo::getPyanzinAnalysisInfo(
      F, FAM.getResult<LoopAnalysis>(F));
}

PreservedAnalyses
PyanzinAnalysisPrinterPass::run(Function &F, FunctionAnalysisManager &AM) {
  OS << "Printing analysis results of CFA for function "
     << "'" << F.getName() << "':"
     << "\n";
  AM.getResult<PyanzinAnalysis>(F).print(OS);
  return PreservedAnalyses::all();
}
