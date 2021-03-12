add_llvm_component_library(LLVMPasses
  PassBuilder.cpp
  PassPlugin.cpp
  StandardInstrumentations.cpp
  

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Passes

  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  aanoskovCounter
  AggressiveInstCombine
  Analysis
  Core
  Coroutines
  HelloNew
  IPO
  InstCombine
  AKomyaginCount
  AvmusatovCount
  ObjCARC
  PyanzinPass1
  Scalar
  Support
  Target
  TransformUtils
  OksanaKozlova
  Vectorize
  Bogoroditskaya
  PankratovaPass
  IDoroshenkoPass
  Instrumentation
  Selivanovskaya_lab1
  IlyinPass
  pazamelin
  MelnikovLab1pass
  MoiseevPass
  KovakimyCount
  BaturinaPass
  VokhmyaninaCounte
  VolokhPass
  Sazanov_Lab2_Pass
  )
