add_llvm_component_library(LLVMSazanov_Lab1_Pass
  Sazanov_Lab1_Pass.cpp
  
  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/transforms/Sazanov_Lab1_Pass
  
  )