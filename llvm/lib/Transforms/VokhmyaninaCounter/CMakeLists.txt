add_llvm_component_library( LLVMVokhmyaninaCounter
  VokhmyaninaCounter.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/VokhmyaninaCounter
) 