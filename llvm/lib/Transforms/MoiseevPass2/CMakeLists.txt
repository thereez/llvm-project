add_llvm_component_library( LLVMMoiseevPass2
  MoiseevPass2.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/MoiseevPass2
) 