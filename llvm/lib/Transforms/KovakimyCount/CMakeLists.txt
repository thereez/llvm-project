add_llvm_component_library(LLVMKovakimyCount
  KovakimyAS.cpp
  KovakimyCount.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/KovakimyCount

)