#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers              //
//  ver 2.3                                                        //
//                                                                 //
// Language:    Visual C++, Visual Studio 2015                     //
// Platform:    Dell Inspiron 7000, Core i5, Windows 10            //
// Application: Spring Projects, 2016                              //
// Author:      Zhihao Zhang,  Syracuse University                 //
//              (479) 595-4305, zzhan154@syr.edu                   //
// Source:		Jim Fawcett                                        //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module builds and configures parsers.  It builds the parser
parts and configures them with application specific rules and actions.

Public Interface:
=================
ConfigParseToConsole conConfig;
conConfig.Build();
conConfig.Attach(someFileName);

Build Process:
==============
Required files
- Parser.h, Parser.cpp, SemiExpression.h, SemiExpression.cpp,
tokenizer.h, tokenizer.cpp
ActionsAndRules.h, ActionsAndRules.cpp,
ConfigureParser.h, ConfigureParser.cpp,
FileMgr.h,FileMgr.cpp,
ConfigureParser.h, ConfigureParser.cpp,
ScopeStack.h, ScopeStack.cpp,

  Build commands
  - devenv ParallelDependencyAnalysis.sln

  Maintenance History:
  ====================
  ver 2.3 : 05 Apr 16
  - add typeTable pointers
  ver 2.2 : 9 Mar 16
  - Added AST for code analysis
  ver 2.1 : 19 Feb 16
  - Added PrintFunction action to FunctionDefinition rule
  ver 2.0 : 01 Jun 11
  - Major revisions to begin building a strong code analyzer
  ver 1.1 : 01 Feb 06
  - cosmetic changes to ConfigureParser.cpp
  ver 1.0 : 12 Jan 06
  - first release

*/

//
#include <fstream>
#include "Parser.h"
#include "SemiExp.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include <map>
#include <unordered_set>

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParseToConsole : IBuilder
{
public:
  ConfigParseToConsole() {};
  ~ConfigParseToConsole();
  bool Attach(const std::string& name, bool isFile=true);
  Parser* buildForTypeAnalysis(std::map<std::string,element>* p_Map);
  Parser* buildForDependenceAnalysis(std::map<std::string, element>* pMergeTable, std::unordered_set<std::string>* pDependencies);
  Parser* Build();

private:
  // Builder must hold onto all the pieces
  std::map<std::string, element>* pMap;
  std::ifstream* pIn;
  Scanner::Toker* pToker;
  Scanner::SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;
  std::map<std::string, element>* mergeMap;
  std::unordered_set<std::string>* dependencySet;



  // add Rules and Actions
  HandleNameSpace* pHandleNameSpace;
  HandleAliases* pHandleAliases;
  HandleTypedefs* pHandleTypedefs;
  BeginningOfScope* pBeginningOfScope;
  HandlePush* pHandlePush;
  EndOfScope* pEndOfScope;
  HandlePop* pHandlePop;
  HandleClass* pHandleClass;
  HandleStruct* pHandleStruct;
  HandleEnum* pHandleEnum;
  FunctionDefinition* pFunctionDefinition;
  PushFunction* pPushFunction;
  PrintFunction* pPrintFunction;
  Declaration* pDeclaration;
  ShowDeclaration* pShowDeclaration;
  Executable* pExecutable;
  ShowExecutable* pShowExecutable;
  FindDependency* pFindDependcy;
  HandleDependecy* pHandleDependcy;


  // prohibit copies and assignments

  ConfigParseToConsole(const ConfigParseToConsole&) = delete;
  ConfigParseToConsole& operator=(const ConfigParseToConsole&) = delete;
};


#endif
