add_llvm_component_library( LLVMSelivanovskaya_count
  Selivanovskaya_count.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/Selivanovskaya_count
) 