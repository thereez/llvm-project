add_llvm_component_library( LLVMVoronovaLab2
  VoronovaLab2.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/VoronovaLab2
)