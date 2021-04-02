add_llvm_component_library( LLVMPyanzinPass3
  PyanzinPass3.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/PyanzinPass3

)