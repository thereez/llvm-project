add_llvm_component_library(LLVMMelnikovLab2Pass
MelnikovLab2Pass.cpp

DEPENDS 
intrinsics_gen

LINK_COMPONENTS
Core
Support
)