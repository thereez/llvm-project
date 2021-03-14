add_llvm_component_library( LLVMBogoroditskaya2
  Bogoroditskaya2.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/Bogoroditskaya2

)
