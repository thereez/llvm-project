#include "llvm/Transforms/BaturinaPass/BaturinaPass.h"
#define DEBUG_TYPE "Baturina pass"

#include "llvm/ADT/Statistic.h"
using namespace llvm;

STATISTIC(definitions_cnt, "Number of function definitions");
STATISTIC(loops_cnt, "Number of loops");
STATISTIC(basic_blocks_counter, "Number of basic blocks");
STATISTIC(add_cnt, "Number of add");
STATISTIC(mul_cnt, "Number of mul");


void loopsCounter(Loop* loops) {
    loops_cnt ++;
    for (auto loop = loops->begin(); loop != loops->end(); loop++) {
        loopsCounter(*loop);
    }
}

PreservedAnalyses BaturinaPass::run(Function &F, FunctionAnalysisManager &FAM) {
    if (!F.isDeclaration()) { definitions_cnt ++; } // if not prototype
    llvm::LoopAnalysis::Result& loops = FAM.getResult<LoopAnalysis>(F);
    for (auto loop = loops.begin(), loops_end = loops.end(); loop != loops_end; loop++) {
            loopsCounter(*loop);
        }
    for (auto block = F.begin(), func_end = F.end(); block != func_end; block++) {
        basic_blocks_counter ++;
        for (auto i = block->begin(), block_end = block->end(); i != block_end; i++) {
            std::string instr = std::string(i->getOpcodeName());
            if (instr == "add" || instr == "fadd") { add_cnt ++; }
            if (instr == "mul" || instr == "fmul") { mul_cnt ++; }
        }
    }
    return PreservedAnalyses::all();
}