add_llvm_component_library(LLVMKatyaMalyshevaLab1Pass
KatyaMalyshevaLab1Pass.cpp

DEPENDS 
intrinsics_gen

LINK_COMPONENTS
Core
Support
) 