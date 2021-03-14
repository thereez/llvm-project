add_llvm_component_library( LLVMPyanzin_ASPass
  Pyanzin_ASPass.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/Pyanzin_ASPass

)