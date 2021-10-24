add_llvm_library( LLVMObfuscation
  CryptoUtils.cpp
  Substitution.cpp
  StringObfuscation.cpp
  BogusControlFlow.cpp
  Utils.cpp
  SplitBasicBlocks.cpp
  Flattening.cpp
  FunctionPassCnf.cpp
  
  PLUGIN_TOOL
  opt
)