/////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ language constructs                  //
//  ver 1.6                                                        //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron 15 7000 Series                    //
//  Application:   Project 3, CSE 687                              //
//  Author:        Zhihao Zhang            Syracuse University     //
//                 (479)595-3948, zzhan154@syr.edu                 //                 
//  Source:		   Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "Tokenizer.h"
#include "SemiExp.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;
using namespace Utilities;

//----< register parsing rule >--------------------------------

void Parser::addRule(IRule* pRule)
{
  rules.push_back(pRule);
}
//----< get next ITokCollection >------------------------------

bool Parser::next() 
{ 
  // default operation of doFold(pTokColl) is to simply return
  // - code analysis provides specific rules for handling
  //   for statements and operator expressions which are
  //   bound in ConfigureParser.cpp

  bool succeeded = pTokColl->get();
  if(!succeeded)
    return false;
  return true;
}

//----< parse the SemiExp by applying all rules to it >--------

bool Parser::parse()
{
  bool succeeded = false;
  for(size_t i=0; i<rules.size(); ++i)
  {
    if(rules[i]->doTest(pTokColl))
      return true;
  }
  return succeeded;
}
//----< register action with a rule >--------------------------

void IRule::addAction(IAction *pAction)
{
  actions.push_back(pAction);
}
//----< invoke all actions associated with a rule >------------

void IRule::doActions(ITokCollection*& pTokColl)
{
  if(actions.size() > 0)
	  for (size_t i = 0; i < actions.size(); ++i)
	  {
		  if(actions[i]->doAction(pTokColl))return;
	  }
}

//----< test stub >--------------------------------------------

#include "../FileMgr/FileSystem.h"

#ifdef TEST_PARSER

#include <queue>
#include <string>
#define Util StringHelper

int main(int argc, char* argv[])
{
  Util::Title("Testing Parser Class", '=');
  putline();

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout  << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::string fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
    std::string msg = "Processing file" + fileSpec;
    Util::title(msg);
    putline();

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << fileSpec << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      while(pParser->next())
        pParser->parse();
      std::cout << "\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
  }
}

#endif
