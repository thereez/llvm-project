add_llvm_component_library(LLVMGeexieAS
  GeexieAS.cpp

  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  Core
  Support
  )
