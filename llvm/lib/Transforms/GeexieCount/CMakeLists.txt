add_llvm_component_library(LLVMGeexieCount
  GeexieCount.cpp

  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  Core
  Support
  )
