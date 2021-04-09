add_llvm_component_library( LLVMIDoroshenkoPass
  idoroshenkoPass.cpp
  idoroshenkoPass2.cpp
  idoroshenkoPass3.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/idoroshenkoPass
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/idoroshenkoPass2
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/idoroshenkoPass3

)