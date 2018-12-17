/*

clang transpiler.cpp -I/usr/local/opt/llvm/include/ -std=c++14 -L/usr/local/opt/llvm/lib/ -lLLVMLTO -lLLVMPasses -lLLVMObjCARCOpts -lLLVMSymbolize -lLLVMDebugInfoPDB -lLLVMDebugInfoDWARF -lLLVMMIRParser -lLLVMFuzzMutate -lLLVMCoverage -lLLVMTableGen -lLLVMDlltoolDriver -lLLVMOrcJIT -lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo -lLLVMXCoreAsmPrinter -lLLVMSystemZDisassembler -lLLVMSystemZCodeGen -lLLVMSystemZAsmParser -lLLVMSystemZDesc -lLLVMSystemZInfo -lLLVMSystemZAsmPrinter -lLLVMSparcDisassembler -lLLVMSparcCodeGen -lLLVMSparcAsmParser -lLLVMSparcDesc -lLLVMSparcInfo -lLLVMSparcAsmPrinter -lLLVMPowerPCDisassembler -lLLVMPowerPCCodeGen -lLLVMPowerPCAsmParser -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMPowerPCAsmPrinter -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMNVPTXAsmPrinter -lLLVMMSP430CodeGen -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMSP430AsmPrinter -lLLVMMipsDisassembler -lLLVMMipsCodeGen -lLLVMMipsAsmParser -lLLVMMipsDesc -lLLVMMipsInfo -lLLVMMipsAsmPrinter -lLLVMLanaiDisassembler -lLLVMLanaiCodeGen -lLLVMLanaiAsmParser -lLLVMLanaiDesc -lLLVMLanaiAsmPrinter -lLLVMLanaiInfo -lLLVMHexagonDisassembler -lLLVMHexagonCodeGen -lLLVMHexagonAsmParser -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMBPFDisassembler -lLLVMBPFCodeGen -lLLVMBPFAsmParser -lLLVMBPFDesc -lLLVMBPFInfo -lLLVMBPFAsmPrinter -lLLVMARMDisassembler -lLLVMARMCodeGen -lLLVMARMAsmParser -lLLVMARMDesc -lLLVMARMInfo -lLLVMARMAsmPrinter -lLLVMARMUtils -lLLVMAMDGPUDisassembler -lLLVMAMDGPUCodeGen -lLLVMAMDGPUAsmParser -lLLVMAMDGPUDesc -lLLVMAMDGPUInfo -lLLVMAMDGPUAsmPrinter -lLLVMAMDGPUUtils -lLLVMAArch64Disassembler -lLLVMAArch64CodeGen -lLLVMAArch64AsmParser -lLLVMAArch64Desc -lLLVMAArch64Info -lLLVMAArch64AsmPrinter -lLLVMAArch64Utils -lLLVMObjectYAML -lLLVMLibDriver -lLLVMOption -lLLVMWindowsManifest -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMGlobalISel -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMLineEditor -lLLVMInterpreter -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMCodeGen -lLLVMTarget -lLLVMCoroutines -lLLVMipo -lLLVMInstrumentation -lLLVMVectorize -lLLVMScalarOpts -lLLVMLinker -lLLVMIRReader -lLLVMAsmParser -lLLVMInstCombine -lLLVMBitWriter -lLLVMAggressiveInstCombine -lLLVMTransformUtils -lLLVMAnalysis -lLLVMProfileData -lLLVMObject -lLLVMMCParser -lLLVMMC -lLLVMDebugInfoCodeView -lLLVMDebugInfoMSF -lLLVMBitReader -lLLVMCore -lLLVMBinaryFormat -lLLVMSupport -lLLVMDemangle -lclangFrontend -lclangSerialization -lclangDriver -lclangTooling -lclangParse -lclangSema -lclangAnalysis -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangAST -lclangLex -lclangBasic -lclangASTMatchers -lcurses -lz -lstdc++

*/


#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class OutputFile {

public:
  OutputFile(std::string ModuleName, std::string Extension) {
    myFileName = ModuleName + "." + Extension;
    myOutputStream.open(myFileName);
  }

  void increaseIndent(int amount) {
    myIndent += amount;
  }

  void decreaseIndent(int amount) {
    myIndent -= amount;
  }
  
  void write(std::string text) {
    if (myIndent > 0 && myCurrentLine.empty()) {
      for (int i = 0; i < myIndent; ++i) {
        myCurrentLine += " ";
      }
    }

    myCurrentLine += text;
  }

  void writeLine(std::string text) {
    write(text);
    newLine();
  }
  
  void newLine() {
    myOutputStream << myCurrentLine << "\n";
    myCurrentLine = "";
  }

  ~OutputFile() {
    myOutputStream << myCurrentLine << std::endl;
    myOutputStream.close();
  }
  
private:
  std::ofstream myOutputStream;
  std::string myCurrentLine;  
  std::string myFileName;
  int myIndent = 0;  
};

class BaseTranspiler {

public:
  virtual void transpile(clang::FunctionDecl* functionDecl) = 0;
  virtual void transpile(clang::VarDecl* variableDecl) = 0;
  virtual void transpile(clang::BinaryOperator* binaryOperator) = 0;
  virtual void transpile(clang::UnaryOperator* unaryOperator) = 0;
  virtual void transpile(clang::DeclRefExpr* expression) = 0;
  virtual void transpile(clang::ImplicitCastExpr* expression) = 0;
  virtual void transpile(clang::IntegerLiteral* integerLiteral) = 0;
  virtual void transpile(clang::CompoundStmt* statement) = 0;
  virtual void transpile(clang::DeclStmt* statement) = 0;
  virtual void transpile(clang::ForStmt* statement) = 0;
  virtual void transpile(clang::ReturnStmt* statement) = 0;

protected:
  bool transpile(clang::Decl* declaration) {
      
    if (clang::isa<clang::FunctionDecl>(declaration)) {
      transpile(clang::cast<clang::FunctionDecl>(declaration));
      return true;
    }
    else if (clang::isa<clang::VarDecl>(declaration)) {
      transpile(clang::cast<clang::VarDecl>(declaration));
      return true;
    }

    return false;
  }

  bool transpile(clang::Expr* expression) {
    if (clang::isa<clang::BinaryOperator>(expression)) {
      transpile(clang::cast<clang::BinaryOperator>(expression));
      return true;
    }
    else if (clang::isa<clang::DeclRefExpr>(expression)) {
      transpile(clang::cast<clang::DeclRefExpr>(expression));
      return true;
    }
    else if (clang::isa<clang::ImplicitCastExpr>(expression)) {
      transpile(clang::cast<clang::ImplicitCastExpr>(expression));
      return true;
    }
    else if (clang::isa<clang::IntegerLiteral>(expression)) {
      transpile(clang::cast<clang::IntegerLiteral>(expression));
      return true;
    }
    else if (clang::isa<clang::UnaryOperator>(expression)) {
      transpile(clang::cast<clang::UnaryOperator>(expression));
      return true;
    }

    return false;
  }

  bool transpile(clang::Stmt* statement) {

    if (clang::isa<clang::CompoundStmt>(statement)) {
      transpile(clang::cast<clang::CompoundStmt>(statement));
      return true;
    }
    else if (clang::isa<clang::DeclStmt>(statement)) {
      transpile(clang::cast<clang::DeclStmt>(statement));
      return true;
    }
    else if (clang::isa<clang::ForStmt>(statement)) {
      transpile(clang::cast<clang::ForStmt>(statement));
      return true;
    }
    else if (clang::isa<clang::ReturnStmt>(statement)) {
      transpile(clang::cast<clang::ReturnStmt>(statement));
      return true;
    }
    else if (clang::isa<clang::Expr>(statement)) {
      return transpile(clang::cast<clang::Expr>(statement));
    }

    return false;
  }
};

class JavaTranspiler : public BaseTranspiler {

public:
  JavaTranspiler(std::string ModuleName) : myModuleName(ModuleName),
                                           myOutputFile(ModuleName, "java")
  {}
  
  void transpile(clang::TranslationUnitDecl* translationUnitDecl) {
    myOutputFile.write("package org.alexandria.");
    myOutputFile.writeLine(myModuleName);
    myOutputFile.newLine();

    myOutputFile.write("public class ");
    myOutputFile.writeLine(myModuleName);
    myOutputFile.writeLine("{");

    myOutputFile.increaseIndent(2);
    
    for (auto i = translationUnitDecl->decls_begin();
         i != translationUnitDecl->decls_end(); ++i) {
      if (transpile(*i)) 
        myOutputFile.newLine();
    }

    myOutputFile.decreaseIndent(2); 

    myOutputFile.writeLine("}");
  }

private:

  using BaseTranspiler::transpile;

  static std::string cppTypeToJavaType(clang::QualType cppType) {
    return cppType.getAsString();
  }
  
  void transpile(clang::FunctionDecl* functionDecl) override
  {
    std::string name =
      functionDecl->getNameInfo().getName().getAsString();

    std::string functionDeclaration = "public static ";

    functionDeclaration += cppTypeToJavaType(functionDecl->getReturnType());
    functionDeclaration += " ";
    functionDeclaration += name;
    functionDeclaration += "(";

    bool isFirst = true;
    
    for (auto i = functionDecl->param_begin();
         i != functionDecl->param_end(); ++i) {
      if (!isFirst) functionDeclaration += ", ";
      functionDeclaration += cppTypeToJavaType((*i)->getOriginalType());
      functionDeclaration += " ";
      functionDeclaration += (*i)->getNameAsString();
    }

    functionDeclaration += ")";
    myOutputFile.writeLine(functionDeclaration);

    myOutputFile.increaseIndent(2);    

    if (functionDecl->hasBody()) {
      transpile(functionDecl->getBody());
    }

    myOutputFile.decreaseIndent(2);

    myOutputFile.newLine();
  }

  void transpile(clang::VarDecl* variableDecl) override {
    if (variableDecl->hasInit()) {
      myOutputFile.write(cppTypeToJavaType(variableDecl->getType()));
      myOutputFile.write(" ");
      myOutputFile.write(variableDecl->getNameAsString());
      myOutputFile.write(" = ");
      transpile(variableDecl->getInit());
    }
  }

  void transpile(clang::BinaryOperator* binaryOperator) override {
    transpile(binaryOperator->getLHS());
    myOutputFile.write(" ");
    myOutputFile.write(binaryOperator->getOpcodeStr());
    myOutputFile.write(" ");
    transpile(binaryOperator->getRHS());
  }

  void transpile(clang::UnaryOperator* unaryOperator) override {
    transpile(unaryOperator->getSubExpr());

    if (unaryOperator->isIncrementOp()) {
      myOutputFile.write("++");
    }
  }
  
  void transpile(clang::DeclRefExpr* expression)
    override {
    myOutputFile.write(expression->getNameInfo().getName().getAsString());
  }
  
  void transpile(clang::ImplicitCastExpr* expression)
    override {
    transpile(expression->getSubExpr());
  }
  
  void transpile(clang::IntegerLiteral* integerLiteral)
    override {
    myOutputFile.write(std::to_string
                       (integerLiteral->getValue().getLimitedValue()));
  }
    
  void transpile(clang::CompoundStmt* statement)
    override {

    myOutputFile.writeLine("{");

    for (auto i = statement->body_begin();
         i != statement->body_end(); ++i) {
      if (transpile(*i)) {
        myOutputFile.write(";");
        myOutputFile.newLine();
      }
    }

    myOutputFile.writeLine("}");    
  }

  void transpile(clang::DeclStmt* statement)
    override {
    if (statement->isSingleDecl()) {
      transpile(statement->getSingleDecl());
    }
  }
  
  void transpile(clang::ForStmt* statement) override {
    myOutputFile.write("for (");
    transpile(statement->getInit());
    myOutputFile.write("; ");
    transpile(statement->getCond());
    myOutputFile.write("; ");
    transpile(statement->getInc());
    myOutputFile.write(")");
    myOutputFile.newLine();

    myOutputFile.increaseIndent(2);    
    transpile(statement->getBody());
    myOutputFile.decreaseIndent(2);
  }

  void transpile(clang::ReturnStmt* statement) override {
    myOutputFile.write("return ");
    transpile(statement->getRetValue());
  }

  std::string myModuleName;
  OutputFile myOutputFile;
};

class PythonTranspiler : public BaseTranspiler {

public:
  PythonTranspiler(std::string ModuleName) : myModuleName(ModuleName),
                                             myOutputFile(ModuleName, "py")
  {}
  
  void transpile(clang::TranslationUnitDecl* translationUnitDecl) {
    for (auto i = translationUnitDecl->decls_begin();
         i != translationUnitDecl->decls_end(); ++i) {
      if (transpile(*i)) 
        myOutputFile.newLine();
    }
  }

private:

  using BaseTranspiler::transpile;
  
  void transpile(clang::FunctionDecl* functionDecl) override
  {
    std::string name =
      functionDecl->getNameInfo().getName().getAsString();

    std::string functionDeclaration = "def ";
    functionDeclaration += name;
    functionDeclaration += "(";

    bool isFirst = true;
    
    for (auto i = functionDecl->param_begin();
         i != functionDecl->param_end(); ++i) {
      if (!isFirst) functionDeclaration += ", ";
      functionDeclaration += (*i)->getNameAsString();
    }

    functionDeclaration += "):";
    myOutputFile.writeLine(functionDeclaration);

    myOutputFile.increaseIndent(2);    

    if (functionDecl->hasBody()) {
      transpile(functionDecl->getBody());
    }
    else {
      myOutputFile.writeLine("pass");
    }

    myOutputFile.decreaseIndent(2);    

    myOutputFile.newLine();
  }

  void transpile(clang::VarDecl* variableDecl) override {
    if (variableDecl->hasInit()) {
      myOutputFile.write(variableDecl->getNameAsString());
      myOutputFile.write(" = ");
      transpile(variableDecl->getInit());
    }
  }

  void transpile(clang::BinaryOperator* binaryOperator) override {
    transpile(binaryOperator->getLHS());
    myOutputFile.write(" ");
    myOutputFile.write(binaryOperator->getOpcodeStr());
    myOutputFile.write(" ");
    transpile(binaryOperator->getRHS());
  }

  void transpile(clang::UnaryOperator* unaryOperator) override {
    transpile(unaryOperator->getSubExpr());

    if (unaryOperator->isIncrementOp()) {
      myOutputFile.write(" += 1");
    }
  }
  
  void transpile(clang::DeclRefExpr* expression)
    override {
    myOutputFile.write(expression->getNameInfo().getName().getAsString());
  }
  
  void transpile(clang::ImplicitCastExpr* expression)
    override {
    transpile(expression->getSubExpr());
  }
  
  void transpile(clang::IntegerLiteral* integerLiteral)
    override {
    myOutputFile.write
      (std::to_string(integerLiteral->getValue().getLimitedValue()));
  }
    
  void transpile(clang::CompoundStmt* statement)
    override {
    for (auto i = statement->body_begin();
         i != statement->body_end(); ++i) {
      if (transpile(*i))
        myOutputFile.newLine();
    }
  }

  void transpile(clang::DeclStmt* statement)
    override {
    if (statement->isSingleDecl()) {
      transpile(statement->getSingleDecl());
    }
  }
  
  void transpile(clang::ForStmt* statement) override {
    // TODO A better implementation would be to use range().
    // Use xrange() for Python 2.x and range() for Python 3.x.
    // TODO: Another advantage of the transpiler is that it can
    // target multiple variants of a language (e.g. Python 2.x/3.x,
    // Perl 5?, 6+?).
    transpile(statement->getInit());
    myOutputFile.newLine();
    myOutputFile.write("while ");
    transpile(statement->getCond());
    myOutputFile.write(":");
    myOutputFile.newLine();

    myOutputFile.increaseIndent(2);    
    transpile(statement->getBody());
    transpile(statement->getInc());
    myOutputFile.decreaseIndent(2);
  }

  void transpile(clang::ReturnStmt* statement) override {
    myOutputFile.write("return ");
    transpile(statement->getRetValue());
  }

  std::string myModuleName;
  OutputFile myOutputFile;
};

class TranspilerConsumer : public clang::ASTConsumer {
public:
  explicit TranspilerConsumer(clang::ASTContext *Context,
                              std::string ModuleName) : myModuleName(ModuleName)
  {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    clang::TranslationUnitDecl *translationUnitDecl =
      Context.getTranslationUnitDecl();

    {
      JavaTranspiler transpiler(myModuleName);
      transpiler.transpile(translationUnitDecl);
    }
    
    {
      PythonTranspiler transpiler(myModuleName);
      transpiler.transpile(translationUnitDecl);
    }
  }

private:
  std::string myModuleName;
};

class TranspilerFrontendAction : public clang::ASTFrontendAction {
public:
  TranspilerFrontendAction(std::string ModuleName) : myModuleName(ModuleName)
  {}
  
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    return std::unique_ptr<clang::ASTConsumer>
      (new TranspilerConsumer(&Compiler.getASTContext(), myModuleName));
  }

private:
  std::string myModuleName;
};

// Usage: ./transpiler <Module.cpp>
int main(int argc, char **argv) {
  if (argc > 1) {
    std::string fileName(argv[1]);
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();

    // TOOD Error if the file name does not contain a dot.
    std::string moduleName;
    const size_t dotOffset = fileName.find_last_of('.');
    if (dotOffset == std::string::npos) {
      moduleName = "unknown";
    }
    else {
      moduleName = fileName.substr(0, dotOffset);
    }
    
    clang::tooling::runToolOnCode(new TranspilerFrontendAction(moduleName),
                                  code);
  }
}
