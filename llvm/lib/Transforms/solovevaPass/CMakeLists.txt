add_llvm_component_library( LLVMsolovevaPass
  solovevaPass.cpp
  solovevaPass2.cpp


  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/solovevaPass
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/solovevaPass2

)