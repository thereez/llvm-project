add_llvm_component_library( LLVMIDoroshenkoPass2
  idoroshenkoPass2.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/idoroshenkoPass2

)