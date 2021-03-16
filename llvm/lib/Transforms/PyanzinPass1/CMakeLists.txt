add_llvm_component_library( LLVMPyanzinPass1
  PyanzinPass1.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/PyanzinPass1

)