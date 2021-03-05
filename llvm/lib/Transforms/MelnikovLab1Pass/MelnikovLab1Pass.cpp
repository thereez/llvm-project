#include "llvm/Transforms/MelnikovLab1Pass/MelnikovLab1Pass.h"

#define DEBUG_TYPE "MelnikovLab1Pass" 

using namespace llvm;

//STATISTIC(TotalFuncs, "Number of functions");
STATISTIC(TotalDefinitions, "Number of definitions"); 
STATISTIC(TotalLoops, "Number of loops");
STATISTIC(TotalBasicBlocks, "Number of basic blocks");
STATISTIC(TotalAdds, "Number of add instructions");
STATISTIC(TotalMulls, "Number of mull instructions");


void count_nested_loops(Loop* loop) {
  for (Loop::iterator inner_loop = loop->begin(), inner_loop_end = loop->end(); inner_loop != inner_loop_end; ++inner_loop) {
    TotalLoops++;
    count_nested_loops(*inner_loop);
  }
}


PreservedAnalyses MelnikovLab1Pass::run(Function &func, FunctionAnalysisManager &AM) {
  //errs() << func.getName() << "\n";
  //TotalFuncs++;

  if (!func.isDeclaration()) { // not declaration => definition
    TotalDefinitions++;
    
    LoopInfo &LI = AM.getResult<LoopAnalysis>(func);

    for (LoopInfo::iterator loop = LI.begin(), loop_end = LI.end(); loop != loop_end; ++loop) {  // iterationg over loops
      TotalLoops++;
      count_nested_loops(*loop);
    }
    
    for (Function::iterator bb = func.begin(), last_bb = func.end(); bb != last_bb; ++bb){ // iterating across basic blocks (bb) in a function (func)
      TotalBasicBlocks++;
      for (BasicBlock::iterator instr = bb->begin(), instr_end = bb->end();  instr != instr_end; ++instr){  // iterating across instructions (instr) in a basic block (bb)
        // outs() << "Instruction: " << *instr << ". Opcode is: " << instr->getOpcode() << "\n";

        // 14 is the fadd instruction opcode
        // 18 is the fmul instruction opcode
        
        if (instr->getOpcode() == 14) { // fadd
          TotalAdds++;
        }
        else if (instr->getOpcode() == 18){ // fmul
          TotalMulls++;
        }
      }
    }
  }
  return PreservedAnalyses::all();
}
