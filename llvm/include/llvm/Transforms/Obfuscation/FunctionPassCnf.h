#ifndef _OBFUSCATION_FUNCTIONPASSCNF_H_
#define _OBFUSCATION_FUNCTIONPASSCNF_H_

#include "llvm/IR/Function.h"
#include "llvm/Support/JSON.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/Local.h" // For DemoteRegToStack and DemotePHIToStack
#include <string>


struct FunctionPassCnf : public llvm::FunctionPass {
  bool flag;
  llvm::json::Value configJson;
  std::set<std::string> obfuscatedFuncs;
  FunctionPassCnf(char ID) : llvm::FunctionPass(ID), configJson(0) {}

  FunctionPassCnf(bool flag, char ID) : llvm::FunctionPass(ID), configJson(0) {
    this->flag = flag;
  }
  virtual bool doInitialization(llvm::Module &M);
  bool toValidateJson();
  bool PrintFunctionAddress(llvm::Function &F);
  bool PrintModuleAddress(llvm::Module &M);

}; // namespace

#endif
