/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 2.3                                                        //
//                                                                 //
// Language:    Visual C++, Visual Studio 2015                     //
// Platform:    Dell Inspiron 7000, Core i5, Windows 10            //
// Application: Spring Projects, 2016                              //
// Author:      Zhihao Zhang,  Syracuse University                 //
//              (479) 595-4305, zzhan154@syr.edu                   //
// Source:		Jim Fawcett                                        //
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <fstream>
#include "Parser.h"
#include "SemiExp.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "../FileMgr/FileMgr.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated
  delete pHandleNameSpace;
  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  delete pHandleClass;
  delete pHandleAliases;
  delete pHandleEnum;
  delete pHandleStruct;
  delete pHandleTypedefs;
  delete pFindDependcy;
  delete pHandleDependcy;
  pIn->close();
  delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
	//std::string fullName= FileSystem::Path::getFullFileSpec(name);
	pRepo->setFileName(name);
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}

//----< build Parser specializes for TypeAnalysis >----------------
Parser * ConfigParseToConsole::buildForTypeAnalysis(std::map<std::string, element>* p_Map)
{
	try
	{
		// add Parser's main parts
		pMap = p_Map;
		pToker = new Toker;
		pToker->returnComments(false);
		pSemi = new SemiExp(pToker);
		pParser = new Parser(pSemi);
		pRepo = new Repository(pToker);

		pBeginningOfScope = new BeginningOfScope();
		/*define actions for beginning of Socpe Rules*/
		pHandlePush = new HandlePush(pRepo);
		pHandleClass = new HandleClass(pRepo);
		pHandleStruct = new HandleStruct(pRepo);
		pHandleEnum = new HandleEnum(pRepo);
		pHandleTypedefs = new HandleTypedefs(pRepo);
		pHandleAliases = new HandleAliases(pRepo);
		pHandleNameSpace = new HandleNameSpace(pRepo);
		pBeginningOfScope->addAction(pHandleAliases);
		pBeginningOfScope->addAction(pHandleClass);
		pBeginningOfScope->addAction(pHandleEnum);
		pBeginningOfScope->addAction(pHandleNameSpace);
		pBeginningOfScope->addAction(pHandleStruct);
		pBeginningOfScope->addAction(pHandleTypedefs);
		pBeginningOfScope->addAction(pHandlePush);
		pParser->addRule(pBeginningOfScope);
		pEndOfScope = new EndOfScope();
		/*define actions for end of scope Rules*/
		pHandlePop = new HandlePop(pRepo, pMap);
		pEndOfScope->addAction(pHandlePop);
		pParser->addRule(pEndOfScope);
		return pParser;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return 0;
	}
}
//----< build Parser specializes for DependenceAnalysis >----------------
Parser * ConfigParseToConsole::buildForDependenceAnalysis(std::map<std::string, element>* pMergeTable, std::unordered_set<std::string>* pDependenciesSet)
{
	try
	{
		// add Parser's main parts
		mergeMap = pMergeTable;
		dependencySet = pDependenciesSet;
		pToker = new Toker;
		pToker->returnComments(false);
		pSemi = new SemiExp(pToker);
		pParser = new Parser(pSemi);
		pRepo = new Repository(pToker);

		pFindDependcy = new FindDependency;
		pHandleDependcy = new HandleDependecy(mergeMap, dependencySet, pRepo);

		pFindDependcy->addAction(pHandleDependcy);
		pParser->addRule(pFindDependcy);

		

		return pParser;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return 0;
	}
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
  try
  {
    pToker = new Toker;
    pToker->returnComments(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);

    // configure to manage scope
    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pParser->addRule(pEndOfScope);

    // configure to detect and act on function definitions

    pFunctionDefinition = new FunctionDefinition;
    pPushFunction = new PushFunction(pRepo);  // no action
    pPrintFunction = new PrintFunction(pRepo);
    pFunctionDefinition->addAction(pPushFunction);
    pFunctionDefinition->addAction(pPrintFunction);
    pParser->addRule(pFunctionDefinition);

    // configure to detect and act on declarations and Executables

    pDeclaration = new Declaration;
    pShowDeclaration = new ShowDeclaration;
    pDeclaration->addAction(pShowDeclaration);
    pExecutable = new Executable;
    pShowExecutable = new ShowExecutable;
    pExecutable->addAction(pShowExecutable);
    pParser->addRule(pExecutable);

    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
