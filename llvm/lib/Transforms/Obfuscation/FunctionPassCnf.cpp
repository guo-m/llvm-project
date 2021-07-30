#include "llvm/Transforms/Obfuscation/FunctionPassCnf.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/FileSystem.h"
#include <sstream>
#include <fstream>

using namespace llvm;
using namespace std;
bool FunctionPassCnf::doInitialization(llvm::Module &M){

    std::string homeDir;
    char* home_dir = getenv("DECLANG_HOME");
    if (home_dir == nullptr) {
      home_dir = getenv("HOME");
    }
    if (home_dir == nullptr) {
      llvm::errs() << "[Frontend]: (Warning) Cannot find $DECLANG_HOME, $HOME or %USERPROFILE%\n";
      llvm::errs().flush();
      std::exit(EXIT_FAILURE);
    }
    homeDir.assign(home_dir);
    homeDir = llvm::sys::path::convert_to_slash(homeDir);

    llvm::sys::fs::create_directory(homeDir+"/.DeClang/");
    std::string configPath = homeDir + "/.DeClang/config.json";

    std::ifstream configFile(configPath);
    if (!configFile) {
      llvm::errs() << "[Frontend]: (Warning) Config file "<< configPath << " not found! Pass won't work'\n";
      this->flag = false;
      return false;
    }

    std::string content(
        (std::istreambuf_iterator<char>(configFile) ),
        (std::istreambuf_iterator<char>() ) );

    // if content length is zero (which means no config file in ~/.DeClang/)
    // abort the rest handling
    if (content.length() == 0) {
      llvm::errs() << "[Frontend]: (Warning) Config "<< configPath << "'s JSON content length is 0, Pass won't work'!\n";
      this->flag = false;
      return false;
    }

    llvm::Expected<llvm::json::Value> expectedConfig = llvm::json::parse(content.c_str());
    //configJson = llvm::json::parse(content.c_str());
    if (llvm::Error EC = expectedConfig.takeError()) {
      llvm::errs() << "[Frontend]: Config Error: llvm::json::parse error \n";
      this->flag = false;
      return false;
    }
    configJson = expectedConfig.get();
    if (!configJson.getAsObject()) {
      llvm::errs() << "[Frontend]: Config Error: JSON is broken\n";
      this->flag = false;
      return false;
    }

    if (!toValidateJson())
    { 
      errs() << "[Frontend]:  Config.Json ValidateJson Failed!!\n";
      std::exit(EXIT_FAILURE);
    }
    // llvm::errs() << "[Frontend]: Read Config JSON Done!!""\n";
    this->flag = true;
    return true;
}

//to validate json config files
bool FunctionPassCnf::toValidateJson() {
  llvm::json::Object *jsonObj = configJson.getAsObject();
  if (jsonObj->getArray("obfuscation")) {
    llvm::json::Array *obfArray = jsonObj->getArray("obfuscation");

    for (auto &obj : *obfArray) {
      if (!obj.getAsObject()->getString("name")) {
        errs() << "[Frontend]: json obfuscation array not have name " << "\n";
        return false;
      }
    }
    // errs() << "[Frontend]: json Validate Json successed !!" << "\n";
    return true;
  }
  else
  {
    errs() << "[Frontend]: json obfuscation not found" << "\n";
    return false;
  }
}