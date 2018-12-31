#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include "yaml-cpp/yaml.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

std::string get(const YAML::Node& node,
                std::string Name) {
  try {
    std::string value = node[Name].as<std::string>();
    if (value == "none") value = "";
    return value;
  }
  catch (const std::exception& Exception) {
    std::cerr << "Setting '" << Name << "' missing or bad type.\n";
    return "";
  }
}

bool isTrue(const std::string& Value) {
  if (Value == "true" || Value == "yes" || Value == "required") return true;
  return false;
}

struct Classes {
  std::string mySupported;
  std::string myIdentifier; // e.g. "class NAME"
  std::string myPrivacy;
  std::string myRequired;
  std::string myPublicIdentifier;
  std::string myPrivateIdentifier;
  std::string myStaticIdentifier;  

  void set(const YAML::Node& Node) {
    mySupported = get(Node, "supported");

    if (isTrue(mySupported)) {
      myIdentifier = get(Node, "identifier");
      myPrivacy = get(Node, "privacy");
      myRequired = get(Node, "required");
      myPublicIdentifier = get(Node, "public-identifier");
      myPrivateIdentifier = get(Node, "private-identifier");
      myStaticIdentifier = get(Node, "static-identifier");
    }
  }
};

struct Functions {
  std::string myDeclaration; // e.g. "def NAME(ARGS):"
  std::string myPublicIdentifier;
  std::string myPrivateIdentifier;  
  std::string myMultipleReturns;
  std::string myOverloading; // e.g. sin(float), sin(double).

  void set(const YAML::Node& Node) {
    myDeclaration = get(Node, "declaration");
    myPublicIdentifier = get(Node, "public-identifier");
    myPrivateIdentifier = get(Node, "private-identifier");        
    myMultipleReturns = get(Node, "multiple-return");
    myOverloading = get(Node, "overloading");
  }
};

struct Variables {
  std::string myIdentifier; // e.g. "var NAME"
  std::string myConstIdentifier; // e.g. const
  std::string myMutableIdentifier; // e.g. mut
  std::string myStaticIdentifier; // e.g. static

  void set(const YAML::Node& Node) {
    myIdentifier = get(Node, "identifier");
    myConstIdentifier = get(Node, "const-identifier");
    myMutableIdentifier = get(Node, "mutable-identifier");
    myStaticIdentifier = get(Node, "static-identifier");        
  }
};

struct Operators {
  std::string myAnd;
  std::string myOr;
  std::string myPrePostIncrement; // e.g. ++i, i++, --i & i--;
  std::string myIncrement; // e.g. i +=
  std::string myOverloading; // e.g. custom a + b
  std::string myCast; // C or function.
  
  void set(const YAML::Node& Node) {
    myAnd = get(Node, "and");
    myOr = get(Node, "or");
    myPrePostIncrement = get(Node, "pre-post-increment");
    myIncrement = get(Node, "increment");
    myOverloading = get(Node, "overloading");
    myCast = get(Node, "cast");
  }
};

struct Statements {
  std::string myIf;
  std::string myElseIf;
  std::string myElse;
  std::string myFor;
  std::string myWhile;
  std::string myReturn;
  std::string mySwitch;
  std::string mySwitchCase;
  std::string mySwitchDefault;

  std::string myTrailer;
  
  void set(const YAML::Node& Node) {
    myIf = get(Node, "if");
    myElseIf = get(Node, "else-if");
    myElse = get(Node, "else");
    myFor = get(Node, "for");
    myWhile = get(Node, "while");
    myReturn = get(Node, "return");
    mySwitch = get(Node, "switch");
    mySwitchCase = get(Node, "switch-case");
    mySwitchDefault = get(Node, "switch-default");    
    myTrailer = get(Node, "trailer");
  }
};

struct Types {
  std::string mySpecified;
  std::string myChar;
  std::string myBoolean;
  std::string myInt8;
  std::string myInt16;
  std::string myInt32;
  std::string myInt64;
  std::string myInt128;  

  std::string myUint8;
  std::string myUint16;
  std::string myUint32;
  std::string myUint64;
  std::string myUint128;  

  std::string myFloat32;
  std::string myFloat64;

  void set(const YAML::Node& Node) {
    mySpecified = get(Node, "specified");

    if (mySpecified != "no") {
      myChar = get(Node, "char");
      myBoolean = get(Node, "boolean");
      myInt8 = get(Node, "int8");
      myInt16 = get(Node, "int16");
      myInt32 = get(Node, "int32");
      myInt64 = get(Node, "int64");
      myInt128 = get(Node, "int128");
      
      myUint8 = get(Node, "uint8");
      myUint16 = get(Node, "uint16");
      myUint32 = get(Node, "uint32");
      myUint64 = get(Node, "uint64");
      myUint128 = get(Node, "uint128");
      
      myFloat32 = get(Node, "float32");
      myFloat64 = get(Node, "float64");
    }
  }
};

struct Language {
  Language(std::string filename) {
    YAML::Node node = YAML::LoadFile(filename);

    myName = get(node, "name");
    myExtension = get(node, "extension");
    myHeaderFile = get(node, "header-file");
    myBlockStart = get(node, "block-start");
    myBlockEnd = get(node, "block-end");    
    myComment = get(node, "comment");
    myModuleIdentifier = get(node, "module-identifier");
    myGenerics = get(node, "generics");
    myGarbageCollection = get(node, "garbage-collection");
    myNamespaceIdentifier = get(node, "namespace-identifier");
    myStructIdentifier = get(node, "struct-identifier");

    myClasses.set(node["classes"]);
    myFunctions.set(node["functions"]);
    myVariables.set(node["variables"]);
    myOperators.set(node["operators"]);
    myStatements.set(node["statements"]);
    myTypes.set(node["types"]);
  }

  std::string myName;
  std::string myExtension;
  std::string myHeaderFile;
  std::string myBlockStart;
  std::string myBlockEnd;
  std::string myComment;
  std::string myModuleIdentifier;
  std::string myGenerics;
  std::string myGarbageCollection;
  std::string myNamespaceIdentifier;
  std::string myStructIdentifier;
  
  Classes myClasses;
  Functions myFunctions;
  Variables myVariables;
  Operators myOperators;
  Statements myStatements;
  Types myTypes;
};

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
  virtual void transpile(clang::FunctionTemplateDecl* functionTemplateDecl) =
    0;  
  virtual void transpile(clang::NamespaceDecl* namespaceDecl) = 0;
  virtual void transpile(clang::VarDecl* variableDecl) = 0;
  
  virtual void transpile(clang::BinaryOperator* binaryOperator) = 0;
  virtual void transpile(clang::CallExpr* callExpression) = 0;
  virtual void transpile(clang::CStyleCastExpr* castExpression) = 0;
  virtual void transpile(clang::ParenExpr* unaryOperator) = 0;
  virtual void transpile(clang::UnaryOperator* unaryOperator) = 0;
  virtual void transpile(clang::DeclRefExpr* expression) = 0;
  virtual void transpile(clang::ImplicitCastExpr* expression) = 0;
  virtual void transpile(clang::FloatingLiteral* floatingLiteral) = 0;
  virtual void transpile(clang::IntegerLiteral* integerLiteral) = 0;

  virtual void transpile(clang::CaseStmt* statement) = 0;
  virtual void transpile(clang::DefaultStmt* statement) = 0;    
  virtual void transpile(clang::CompoundStmt* statement) = 0;
  virtual void transpile(clang::DeclStmt* statement) = 0;
  virtual void transpile(clang::ForStmt* statement) = 0;
  virtual void transpile(clang::IfStmt* statement) = 0;
  virtual void transpile(clang::ReturnStmt* statement) = 0;
  virtual void transpile(clang::SwitchStmt* statement) = 0;  

protected:
  bool transpile(clang::Decl* declaration) {
      
    if (clang::isa<clang::FunctionDecl>(declaration)) {
      transpile(clang::cast<clang::FunctionDecl>(declaration));
      return true;
    }
    if (clang::isa<clang::FunctionTemplateDecl>(declaration)) {
      transpile(clang::cast<clang::FunctionTemplateDecl>(declaration));
      return true;
    }
    else if (clang::isa<clang::VarDecl>(declaration)) {
      transpile(clang::cast<clang::VarDecl>(declaration));
      return true;
    }
    else if (clang::isa<clang::NamespaceDecl>(declaration)) {
      transpile(clang::cast<clang::NamespaceDecl>(declaration));
      return true;
    }

    return false;
  }

  bool transpile(clang::Expr* expression) {
    if (clang::isa<clang::BinaryOperator>(expression)) {
      transpile(clang::cast<clang::BinaryOperator>(expression));
      return true;
    }
    else if (clang::isa<clang::CallExpr>(expression)) {
      transpile(clang::cast<clang::CallExpr>(expression));
      return true;
    }
    else if (clang::isa<clang::CStyleCastExpr>(expression)) {
      transpile(clang::cast<clang::CStyleCastExpr>(expression));
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
    else if (clang::isa<clang::FloatingLiteral>(expression)) {
      transpile(clang::cast<clang::FloatingLiteral>(expression));
      return true;
    }
    else if (clang::isa<clang::IntegerLiteral>(expression)) {
      transpile(clang::cast<clang::IntegerLiteral>(expression));
      return true;
    }
    else if (clang::isa<clang::ParenExpr>(expression)) {
      transpile(clang::cast<clang::ParenExpr>(expression));
      return true;
    }
    else if (clang::isa<clang::UnaryOperator>(expression)) {
      transpile(clang::cast<clang::UnaryOperator>(expression));
      return true;
    }

    return false;
  }

  bool transpile(clang::Stmt* statement) {

    if (clang::isa<clang::CaseStmt>(statement)) {
      transpile(clang::cast<clang::CaseStmt>(statement));
      return true;
    }    
    else if (clang::isa<clang::CompoundStmt>(statement)) {
      transpile(clang::cast<clang::CompoundStmt>(statement));
      return true;
    }
    else if (clang::isa<clang::DeclStmt>(statement)) {
      transpile(clang::cast<clang::DeclStmt>(statement));
      return true;
    }
    else if (clang::isa<clang::DefaultStmt>(statement)) {
      transpile(clang::cast<clang::DefaultStmt>(statement));
      return true;
    }    
    else if (clang::isa<clang::ForStmt>(statement)) {
      transpile(clang::cast<clang::ForStmt>(statement));
      return true;
    }
    else if (clang::isa<clang::IfStmt>(statement)) {
      transpile(clang::cast<clang::IfStmt>(statement));
      return true;
    }
    else if (clang::isa<clang::ReturnStmt>(statement)) {
      transpile(clang::cast<clang::ReturnStmt>(statement));
      return true;
    }
    else if (clang::isa<clang::SwitchStmt>(statement)) {
      transpile(clang::cast<clang::SwitchStmt>(statement));
      return true;
    }
    else if (clang::isa<clang::Expr>(statement)) {
      return transpile(clang::cast<clang::Expr>(statement));
    }

    return false;
  }
};

// This is a generic language transpiler based off of the YAML configuration
// file.
class GenericTranspiler : public BaseTranspiler {
public:
  GenericTranspiler(Language language,
                    std::string ModuleName) : myLanguage(language),
                                              myModuleName(ModuleName),
                                              myOutputFile(ModuleName,
                                                           language.myExtension)
  {}

  void transpile(clang::TranslationUnitDecl* translationUnitDecl) {
    // Does the language require a module or package identier?
    if (myLanguage.myModuleIdentifier != "") {
      std::string moduleIdentifier = myLanguage.myModuleIdentifier;
      moduleIdentifier = std::regex_replace(moduleIdentifier,
                                            std::regex("NAME"),
                                            myModuleName);
      myOutputFile.writeLine(moduleIdentifier);
      myOutputFile.newLine();
    }

    // Does this language require everything to be in a class?
    if (isTrue(myLanguage.myClasses.myRequired)) {
      std::string classIdentifier = myLanguage.myClasses.myIdentifier;
      classIdentifier = std::regex_replace(classIdentifier,
                                           std::regex("NAME"),
                                           myModuleName);
      if (myLanguage.myClasses.myPublicIdentifier != "") {
        classIdentifier = myLanguage.myClasses.myPublicIdentifier + " " +
          classIdentifier;
      }

      myOutputFile.write(classIdentifier);
      myOutputFile.writeLine(myLanguage.myBlockStart);
      myOutputFile.increaseIndent(2);
    }
    
    for (auto i = translationUnitDecl->decls_begin();
         i != translationUnitDecl->decls_end(); ++i) {
      if (transpile(*i)) 
        myOutputFile.newLine();
    }

    if (isTrue(myLanguage.myClasses.myRequired)) {    
      myOutputFile.decreaseIndent(2); 
      myOutputFile.writeLine(myLanguage.myBlockEnd);
    }
  }

private:
  
  using BaseTranspiler::transpile;

  std::string cppTypeToTargetType(clang::QualType cppType) {
    const std::string cppTypeString =
      cppType.getUnqualifiedType().getAsString();
    std::string targetTypeString;

    if (cppType.isConstQualified()) {
      if (myLanguage.myVariables.myConstIdentifier != "") {
        myOutputFile.write(myLanguage.myVariables.myConstIdentifier);
        myOutputFile.write(" ");
      }
    }
    else if (myLanguage.myVariables.myMutableIdentifier != "") {
      myOutputFile.write(myLanguage.myVariables.myMutableIdentifier);
      myOutputFile.write(" ");
    }
        
    if (cppTypeString == "char") targetTypeString = myLanguage.myTypes.myChar;
    else if (cppTypeString == "bool")
      targetTypeString = myLanguage.myTypes.myBoolean;

    else if (cppTypeString == "int8_t")
      targetTypeString = myLanguage.myTypes.myInt8;
    else if (cppTypeString == "int16_t" ||
             cppTypeString == "short")
      targetTypeString = myLanguage.myTypes.myInt16;
    else if (cppTypeString == "int32_t" ||
             cppTypeString == "int")
      targetTypeString = myLanguage.myTypes.myInt32;  
    else if (cppTypeString == "int64_t" ||
             cppTypeString == "long")
      targetTypeString = myLanguage.myTypes.myInt64;  
    else if (cppTypeString == "int128_t")
      targetTypeString = myLanguage.myTypes.myInt128;  
    
    else if (cppTypeString == "uint8_t")
      targetTypeString = myLanguage.myTypes.myUint8;
    else if (cppTypeString == "uint16_t" ||
             cppTypeString == "unsigned short")
      targetTypeString = myLanguage.myTypes.myUint16;
    else if (cppTypeString == "uint32_t" ||
             cppTypeString == "unsigned int")
      targetTypeString = myLanguage.myTypes.myUint32;  
    else if (cppTypeString == "uint64_t" ||
             cppTypeString == "unsigned long")
      targetTypeString = myLanguage.myTypes.myUint64;  
    else if (cppTypeString == "uint128_t")
      targetTypeString = myLanguage.myTypes.myUint128;  

    else if (cppTypeString == "float")
      targetTypeString = myLanguage.myTypes.myFloat32;
    else if (cppTypeString == "double")
      targetTypeString = myLanguage.myTypes.myFloat64;  
    else {
      // TODO
    }
    
    if (targetTypeString == "none") {
      // TODO if the type is not supported then don't emit that function.
    }

    return targetTypeString;
  }

  std::string transpileFunctionArguments(clang::FunctionDecl* functionDecl) {
    std::string arguments;
    bool isFirst = true;
    
    for (auto i = functionDecl->param_begin();
         i != functionDecl->param_end(); ++i) {
      if (!isFirst) arguments += ", ";
      if (myLanguage.myTypes.mySpecified != "no") {
        arguments += cppTypeToTargetType((*i)->getOriginalType());
        arguments += " ";
      }
      arguments += (*i)->getNameAsString();
      isFirst = false;
    }

    return arguments;
  }

  void transpile(clang::FunctionDecl* functionDecl) override
  {
    std::string name =
      functionDecl->getNameInfo().getName().getAsString();

    bool isClass = false;
    
    // Does this language require everything to be in a class?
    if (isTrue(myLanguage.myClasses.myRequired)) {
      // Yes. In which case this function will be in a clas.
      isClass = true;
    }

    std::string functionDeclaration;

    if (isClass &&
        myLanguage.myClasses.myStaticIdentifier != "") {
      functionDeclaration += myLanguage.myClasses.myStaticIdentifier;
      functionDeclaration += " ";      
    }

    if (myLanguage.myFunctions.myPublicIdentifier != "") {
      functionDeclaration += myLanguage.myFunctions.myPublicIdentifier;
      functionDeclaration += " ";
    }
    
    if (myLanguage.myFunctions.myDeclaration != "auto") {
      std::string declaration = myLanguage.myFunctions.myDeclaration;
      declaration = std::regex_replace(declaration,
                                       std::regex("NAME"),
                                       name);
      std::string arguments = transpileFunctionArguments(functionDecl);
      declaration = std::regex_replace(declaration,
                                       std::regex("ARGS"),
                                       arguments);
      functionDeclaration += declaration;
    }
    else {
      if (myLanguage.myTypes.mySpecified != "no") {
        functionDeclaration +=
          cppTypeToTargetType(functionDecl->getReturnType());
        functionDeclaration += " ";
      }
      
      functionDeclaration += name;
      functionDeclaration += "(";
      functionDeclaration += transpileFunctionArguments(functionDecl);
      functionDeclaration += ")";
    }

    myOutputFile.write(functionDeclaration);
    myOutputFile.increaseIndent(2);    
    
    if (functionDecl->hasBody()) {
      transpile(functionDecl->getBody());
    }
    
    myOutputFile.decreaseIndent(2);
      
    myOutputFile.newLine();
  }

  void transpile(clang::FunctionTemplateDecl* functionDecl) override {

    std::vector<std::string> parameters = {"DOUBLE"};
    
    for (auto i = functionDecl->spec_begin();
         i != functionDecl->spec_end(); ++i) {
      // TODO: Extract template parameter names from source code.
    }

    // TODO
    transpile(functionDecl->getTemplatedDecl());
  }
  
  void transpile(clang::NamespaceDecl* namespaceDecl) override {

    // MYTODO: Write out namespace header...

    for (auto i = namespaceDecl->decls_begin();
         i != namespaceDecl->decls_end(); ++i) {
      if (transpile(*i)) 
        myOutputFile.newLine();
    }
  }
  
  void transpile(clang::VarDecl* variableDecl) override {
    // If this variable isn't declared and this langauge has no
    // variable identifier or requires specifying variable types, then
    // we can ignore the variable declaration.
    if (!variableDecl->hasInit() &&
        myLanguage.myVariables.myIdentifier == "" &&
        myLanguage.myTypes.mySpecified == "no") return;

    if (myLanguage.myVariables.myIdentifier != "") {
      myOutputFile.write(myLanguage.myVariables.myIdentifier);
      myOutputFile.write(" ");
    }
    if (myLanguage.myVariables.myStaticIdentifier != "" &&
        variableDecl->getStorageDuration() ==
        clang::StorageDuration::SD_Static) {
      myOutputFile.write(myLanguage.myVariables.myStaticIdentifier);
      myOutputFile.write(" ");
    }

    if (myLanguage.myTypes.mySpecified != "no") {
      myOutputFile.write(cppTypeToTargetType(variableDecl->getType()));
      myOutputFile.write(" ");
    }

    myOutputFile.write(variableDecl->getNameAsString());
    
    if (variableDecl->hasInit()) {
      myOutputFile.write(" = ");
      transpile(variableDecl->getInit());
      myOutputFile.write(myLanguage.myStatements.myTrailer);
    }
  }

  void transpile(clang::BinaryOperator* binaryOperator) override {
    transpile(binaryOperator->getLHS());
    myOutputFile.write(" ");
    myOutputFile.write(binaryOperator->getOpcodeStr());
    myOutputFile.write(" ");
    transpile(binaryOperator->getRHS());
  }

  void transpile(clang::CallExpr* callExpression) override {

    clang::FunctionDecl* functionDeclaration =
      callExpression->getDirectCallee();
    if (functionDeclaration != nullptr) {
      myOutputFile.write(functionDeclaration->getNameInfo().getName().
                         getAsString());
    }
    else {
      myOutputFile.write("???");
    }
    
    myOutputFile.write("(");

    bool isFirst = true;
    
    for (auto i = callExpression->arg_begin();
         i != callExpression->arg_end(); ++i) {
      if (!isFirst) myOutputFile.write(", ");
      transpile(*i);
      isFirst = false;
    }

    myOutputFile.write(")");    
  }
  
  void transpile(clang::CStyleCastExpr* castExpr) override {
    const clang::QualType castType = castExpr->getTypeAsWritten();

    if (myLanguage.myOperators.myCast == "C") {
      myOutputFile.write("(");
      myOutputFile.write(cppTypeToTargetType(castType));
      myOutputFile.write(")");
      transpile(castExpr->getSubExpr());
    }
    else {
      if (myLanguage.myTypes.mySpecified != "no") {
        myOutputFile.write(cppTypeToTargetType(castType));
      }
      else {
        myOutputFile.write(castType.getUnqualifiedType().getAsString());
      }
      myOutputFile.write("(");
      transpile(castExpr->getSubExpr());      
      myOutputFile.write(")");            
    }
  }
  
  void transpile(clang::ParenExpr* parenExpr) override {
    myOutputFile.write("(");
    transpile(parenExpr->getSubExpr());
    myOutputFile.write(")");    
  }
  
  void transpile(clang::UnaryOperator* unaryOperator) override {

    // TODO: How do we properly query for prefix "-"?
    const std::string opCodeString = 
      clang::UnaryOperator::getOpcodeStr(unaryOperator->getOpcode());
    
    if (unaryOperator->isIncrementOp()) {
      transpile(unaryOperator->getSubExpr());

      if (isTrue(myLanguage.myOperators.myPrePostIncrement)) {
        myOutputFile.write("++");
      }
      else if (isTrue(myLanguage.myOperators.myIncrement)) {
        myOutputFile.write(" += 1");
      }
      else {
        // TODO: Not yet supported.
      }
    }
    else if (unaryOperator->isPrefix() ||
             opCodeString == "-") {
      myOutputFile.write("-");
      transpile(unaryOperator->getSubExpr());
    }
    else {
      // TODO: Not Supported
      transpile(unaryOperator->getSubExpr());
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

  void transpile(clang::FloatingLiteral* floatingLiteral)
    override {

    std::ostringstream stream;
    stream << std::setprecision(21);
    stream << floatingLiteral->getValue().convertToDouble();
    myOutputFile.write(stream.str());
  }
  
  void transpile(clang::CompoundStmt* statement)
    override {

    myOutputFile.writeLine(myLanguage.myBlockStart);

    for (auto i = statement->body_begin();
         i != statement->body_end(); ++i) {
      if (transpile(*i)) {
        myOutputFile.write(myLanguage.myStatements.myTrailer);
        myOutputFile.newLine();
      }
    }

    myOutputFile.writeLine(myLanguage.myBlockEnd);
  }

  void transpile(clang::DeclStmt* statement)
    override {
    if (statement->isSingleDecl()) {
      transpile(statement->getSingleDecl());
    }
    else {
      const auto& declGroup = statement->getDeclGroup().getDeclGroup();

      for (int i = 0; i < declGroup.size(); i++) {
        transpile(declGroup[i]);
        myOutputFile.write(myLanguage.myStatements.myTrailer);
        myOutputFile.newLine();        
      }
    }
  }
  
  void transpile(clang::ForStmt* statement) override {
    // Does this language support for statements?
    if (myLanguage.myStatements.myFor != "") {
      // Yes.
      const std::string forStatement = myLanguage.myStatements.myFor;
 
      const size_t initialOffset = forStatement.find("INITIAL");
      myOutputFile.write(forStatement.substr(0, initialOffset));
      transpile(statement->getInit());

      const size_t conditionOffset = forStatement.find("CONDITION");      
      size_t startOffset = initialOffset + strlen("INITIAL");
      myOutputFile.write(forStatement.substr(startOffset,
                                             conditionOffset - startOffset));
      transpile(statement->getCond());

      const size_t iterationOffset = forStatement.find("ITERATION");      
      startOffset = conditionOffset + strlen("CONDITION");
      myOutputFile.write(forStatement.substr(startOffset,
                                             iterationOffset - startOffset));
      transpile(statement->getInc());

      startOffset = iterationOffset + strlen("ITERATION");
      myOutputFile.write(forStatement.substr(startOffset,
                                             forStatement.length() -
                                             startOffset));

      myOutputFile.increaseIndent(2);    
      transpile(statement->getBody());
      myOutputFile.decreaseIndent(2);
    }
    else {
      // No. Convert the for-loop to a while-loop.
      transpile(statement->getInit());
      myOutputFile.newLine();

      const std::string whileStatement = myLanguage.myStatements.myWhile;

      const size_t initialOffset = whileStatement.find("CONDITION");
      myOutputFile.write(whileStatement.substr(0, initialOffset));

      transpile(statement->getCond());

      size_t startOffset = initialOffset + strlen("CONDITION");
      myOutputFile.write(whileStatement.substr(startOffset,
                                               whileStatement.length() -
                                               startOffset));
      
      myOutputFile.increaseIndent(2);    
      transpile(statement->getBody());
      transpile(statement->getInc());
      myOutputFile.decreaseIndent(2);
    }
  }

  void transpile(clang::IfStmt* statement, bool isElseIf) {
    std::string ifStatement = (isElseIf? myLanguage.myStatements.myElseIf :
                               myLanguage.myStatements.myIf);
    
    const size_t initialOffset = ifStatement.find_first_of("CONDITION");
    myOutputFile.write(ifStatement.substr(0, initialOffset));
    transpile(statement->getCond());
    myOutputFile.write(ifStatement.substr(initialOffset + strlen("CONDITION"),
                                          ifStatement.length()));

    myOutputFile.increaseIndent(2);
    if (!clang::isa<clang::CompoundStmt>(statement->getThen()))
      myOutputFile.write(" ");
    
    transpile(statement->getThen());
    myOutputFile.write(myLanguage.myStatements.myTrailer);
      
    myOutputFile.decreaseIndent(2);
    myOutputFile.newLine();        

    if (statement->getElse() != nullptr) {
      if (clang::isa<clang::IfStmt>(statement->getElse())) {
        transpile(clang::cast<clang::IfStmt>(statement->getElse()), true);
      }
      else {
        std::string elseStatement = myLanguage.myStatements.myElse;
        myOutputFile.write(elseStatement);
        myOutputFile.write(" ");

        myOutputFile.increaseIndent(2);
        transpile(statement->getElse());
        myOutputFile.decreaseIndent(2);        
      }
    }
  }
    
  void transpile(clang::IfStmt* statement) override {
    transpile(statement, false);
  }

  void transpile(clang::ReturnStmt* statement) override {
    std::string returnStatement = myLanguage.myStatements.myReturn;
    
    const size_t initialOffset = returnStatement.find_first_of("VALUE");
    myOutputFile.write(returnStatement.substr(0, initialOffset));
    transpile(statement->getRetValue());

    myOutputFile.write(returnStatement.substr(initialOffset + strlen("VALUE"),
                                              returnStatement.length()));
  }

  void transpile(clang::CaseStmt* statement) override {
    // We should only get here if our target language supports the switch
    // statement.
    const std::string caseStatement = myLanguage.myStatements.mySwitchCase;

    const size_t initialOffset = caseStatement.find("VALUE");
    myOutputFile.write(caseStatement.substr(0, initialOffset));
    transpile(statement->getLHS());
    
    const size_t startOffset = initialOffset + strlen("VALUE");
    myOutputFile.write(caseStatement.substr(startOffset,
                                            caseStatement.length() -
                                            startOffset));
    myOutputFile.write(" ");    
    myOutputFile.increaseIndent(2);
    transpile(statement->getSubStmt());
    myOutputFile.decreaseIndent(2);      
  }

  void transpile(clang::DefaultStmt* statement) override {
    // We should only get here if our target language supports the switch
    // statement.
    myOutputFile.write(myLanguage.myStatements.mySwitchDefault);
    myOutputFile.write(" ");    
    myOutputFile.increaseIndent(2);
    transpile(statement->getSubStmt());
    myOutputFile.decreaseIndent(2);      
  }
  
  void transpile(clang::SwitchStmt* statement) override {
    // Does this langauge support switch statements?
    if (myLanguage.myStatements.mySwitch != "") {
      // Yes.
      const std::string switchStatement = myLanguage.myStatements.mySwitch;

      const size_t initialOffset = switchStatement.find("CONDITION");
      myOutputFile.write(switchStatement.substr(0, initialOffset));
      transpile(statement->getCond());

      const size_t startOffset = initialOffset + strlen("CONDITION");
      myOutputFile.write(switchStatement.substr(startOffset,
                                                switchStatement.length() -
                                                startOffset));
      myOutputFile.increaseIndent(2);    
      transpile(statement->getBody());      
      myOutputFile.decreaseIndent(2);      
    }
    else {
      clang::CompoundStmt* compoundStatement =
        clang::cast<clang::CompoundStmt>(statement->getBody());
      bool isFirst = true;
      
      for (auto i = compoundStatement->body_begin();
           i != compoundStatement->body_end(); ++i) {
        if (clang::isa<clang::CaseStmt>(*i)) {
          clang::CaseStmt* caseStatement =
            clang::cast<clang::CaseStmt>(*i);

          std::string ifStatement = (isFirst? myLanguage.myStatements.myIf :
                                     myLanguage.myStatements.myElseIf);
          
          const size_t initialOffset = ifStatement.find("CONDITION");
          myOutputFile.write(ifStatement.substr(0, initialOffset));
          transpile(statement->getCond());
          myOutputFile.write(" == ");
          transpile(caseStatement->getLHS());
          
          const size_t startOffset = initialOffset + strlen("CONDITION");
          myOutputFile.write(ifStatement.substr(startOffset,
                                                ifStatement.length() -
                                                startOffset));
          myOutputFile.write(" ");
          
          myOutputFile.increaseIndent(2);    
          transpile(caseStatement->getSubStmt());      
          myOutputFile.decreaseIndent(2);
          myOutputFile.newLine();
        }
        else {
          clang::DefaultStmt* defaultStatement =
            clang::cast<clang::DefaultStmt>(*i);

          myOutputFile.write(myLanguage.myStatements.myElse);
          myOutputFile.write(" ");
 
          myOutputFile.increaseIndent(2);
          transpile(defaultStatement->getSubStmt());
          myOutputFile.decreaseIndent(2);
          myOutputFile.newLine();          
        }

        isFirst = false;
      }
    }
  }
  
  Language myLanguage;
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

    // TODO Traverse language directory automatically.
    {
      Language language("languages/java.yaml");
      GenericTranspiler transpiler(language, myModuleName);
      transpiler.transpile(translationUnitDecl);
    }

    {
      Language language("languages/python.yaml");
      GenericTranspiler transpiler(language, myModuleName);
      transpiler.transpile(translationUnitDecl);
    }

    {
      Language language("languages/c.yaml");
      GenericTranspiler transpiler(language, myModuleName);
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

    // Remove suffix, e.g. ".cpp".
    std::string moduleName;
    const size_t dotOffset = fileName.find_last_of('.');
    if (dotOffset == std::string::npos) {
      moduleName = "unknown";
    }
    else {
      moduleName = fileName.substr(0, dotOffset);
    }

    const size_t slashOffset = moduleName.find_last_of('/');
    if (slashOffset != std::string::npos) {
      moduleName = moduleName.substr(slashOffset + 1, moduleName.length());
    }

    //clang::tooling::CommonOptionsParser parser;
    //clang::tooling::ClangTool Tool;
    
    clang::tooling::runToolOnCode(new TranspilerFrontendAction(moduleName),
                                  code);
  }
}
