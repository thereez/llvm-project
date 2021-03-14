add_llvm_component_library(LLVMArivanovLab2
  ArivanovLab2.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/ArivanovLab2

) 