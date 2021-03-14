add_llvm_component_library(LLVMMelnikovLab1Pass
MelnikovLab1Pass.cpp

DEPENDS 
intrinsics_gen

LINK_COMPONENTS
Core
Support
)