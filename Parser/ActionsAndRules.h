#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 2.3                                                        //
//  Language:      Visual Studio C++ 2015                          //
//  Platform:      Dell Inspiron 7000, Windows 10                  //
//  Application:    for CSE687 Pr2, Spring 2016                    //
//  Author:        Zhihao Zhang, Syracuse University               //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines several action classes.  Its classes provide
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable.

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();               //   and parse it

Build Process:
==============
Required files
- Parser.h, Parser.cpp,
SemiExp.h,SemiExp.cpp
tokenizer.h, tokenizer.cpp, itokcollection.h
ActionsAndRules.h, ActionsAndRules.cpp,
ConfigureParser.h, ConfigureParser.cpp,
ConfigureParser.h, ConfigureParser.cpp,
ScopeStack.h, ScopeStack.cpp,

Build commands
- devenv ParallelDependencyAnalysis.sln

Maintenance History:
====================
ver 2.3 : 04 Apr 16
- add addition rules and actions for type analysis and dependency analysis
ver 2.2 : 10 Mar 16
- delete some old rules and actions, and add some new rules and actions for project2
ver 2.1 : 15 Feb 16
- small functional change to a few of the actions changes display strategy
- preface the (new) Toker and SemiExp with Scanner namespace
ver 2.0 : 01 Jun 11
- added processing on way to building strong code analyzer
ver 1.1 : 17 Jan 09
- changed to accept a pointer to interfaced ITokCollection instead
of a SemiExpression
ver 1.0 : 12 Jan 06
- first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "itokcollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExp.h"
#include <map>
#include <thread>
#include <unordered_set>

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific
/* ToDo:
 * - chanage lineCount to two fields: lineCountStart and lineCountEnd
 */
struct element
{
	element() {
		std::stringstream ss;
		ss << std::this_thread::get_id();
		threadId = ss.str();
	};
	std::string type;
	std::string name;
	std::string fileName;
	std::string threadId;
	size_t lineCount;
};

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
/*
 * ToDo:
 * - add AST Node class
 * - provide field to hold root of AST
 * - provide interface to access AST
 * - provide helper functions to build AST, perhaps in Tree class
 */
class Repository  // application specific
{
private:
	std::string fileName;
  ScopeStack<element> stack;
  Scanner::Toker* p_Toker;
public:
  Repository(Scanner::Toker* pToker)
  {
    p_Toker = pToker;
  }
  void setFileName(const std::string name)
  {
	  fileName = name;
  }
  std::string getFileName()
  {
	  return fileName;
  }
  ScopeStack<element>& scopeStack()
  {
    return stack;
  }
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->currentLineCount());
  }
};


class HandleAliases : public IAction
{
private:
	Repository* p_Repos;
public:
	HandleAliases(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool doAction(ITokCollection*& pTc)
	{
		size_t index = pTc->find("using");
		if (int(index) == 1)
		{
			element elem;
			elem.fileName = p_Repos->getFileName();
			elem.type = "using";
			elem.name = (*pTc)[index+1];
			p_Repos->scopeStack().push(elem);
			return true;
		}
		return false;
	}
};
class HandleTypedefs : public IAction
{
private:
	Repository* p_Repos;
public:
	HandleTypedefs(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool doAction(ITokCollection*& pTc)
	{
		size_t index = pTc->find("typedef");
		if (int(index)==1)
		{
			element elem;
			elem.fileName = p_Repos->getFileName();
			elem.type = "typedef";
			elem.name = (*pTc)[pTc->length()-1];
			p_Repos->scopeStack().push(elem);
			return true;
		}
		return false;
	}
};
class HandleNameSpace : public IAction
{
private:
	Repository* p_Repos;
public:
	HandleNameSpace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool doAction(ITokCollection*& pTc)
	{
		size_t index = pTc->find("namespace");
		if (int(index)<pTc->length())
		{
			element elem;
			elem.fileName = p_Repos->getFileName();
			elem.type = "namespace";
			elem.name = (*pTc)[index + 1];
			p_Repos->scopeStack().push(elem);
			return true;
		}
		return false;
	}
};
class HandleEnum : public IAction
{
private:
	Repository* p_Repos;
public:

	HandleEnum(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool doAction(ITokCollection*& pTc)
	{
		size_t index = pTc->find("enum");
		if (index<pTc->length())
		{
			element elem;
			elem.fileName = p_Repos->getFileName();
			elem.type = "enum";
			elem.name = (*pTc)[index + 1];
			p_Repos->scopeStack().push(elem);
			return true;
		}
		return false;
	}
};

class HandleStruct : public IAction
{
private:
	Repository* p_Repos;
public:

	HandleStruct(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool doAction(ITokCollection*& pTc)
	{
		size_t index = pTc->find("struct");
		if (index<pTc->length())
		{
			element elem;
			elem.fileName = p_Repos->getFileName();
			elem.type = "struct";
			elem.name = (*pTc)[index + 1];
			p_Repos->scopeStack().push(elem);
			return true;
		}
		return false;
	}
};

class HandleClass : public IAction
{
private:
	Repository* p_Repos;
public:

	HandleClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool doAction(ITokCollection*& pTc)
	{
		size_t index = pTc->find("class");
		if (index<pTc->length())
		{
			element elem;
			elem.fileName = p_Repos->getFileName();
			elem.type = "class";
			elem.name = (*pTc)[index + 1];
			p_Repos->scopeStack().push(elem);
			return true;
		}
		return false;
	}
};

// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n--BeginningOfScope rule";
		element elem;
		elem.type = "unknown";
		elem.name = "anonymous";
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
		return true;

	}
};
///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return false;
  }
};
//------ find depency of the file
class FindDependency : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
			doActions(pTc);
			return true;
	}
};

class HandleDependecy : public IAction
{
private:
	std::map<std::string, element>* pMergeTable;
	std::unordered_set<std::string>* pDependencies;
	Repository* p_Repos;
public:
	HandleDependecy(std::map<std::string, element>* mergeTable, std::unordered_set<std::string>* dependencies, Repository* repos)
	{
		pMergeTable = mergeTable;
		pDependencies = dependencies;
		p_Repos = repos;
	}
	bool doAction(ITokCollection*& pTc)
	{
		size_t len = pTc->length();
		for (size_t i = 0; i < len; i++)
		{
			if (pMergeTable->find((*pTc)[i]) != pMergeTable->end()&& (*pMergeTable)[(*pTc)[i]].fileName.compare(p_Repos->getFileName())!=0)
			{
				
				pDependencies->insert((*pMergeTable)[(*pTc)[i]].fileName);
			}
		}
		return true;

	}
};

///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
private:
	std::map<std::string, element>* typeTable;
	Repository* p_Repos;
public:
  HandlePop(Repository* pRepos, std::map<std::string, element>* p_typeTable)
  {
	typeTable = p_typeTable;
    p_Repos = pRepos;
  }

  bool doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if(p_Repos->scopeStack().size() == 0)
      return false;
    element elem = p_Repos->scopeStack().pop();
    if(elem.type != "unknown")
    {
		(*typeTable)[elem.name] = elem;
    }
	return true;
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
  bool doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]))
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  bool doAction(ITokCollection*& pTc)
  {
    // next statement is now  handled in PrintFunction
    // std::cout << "\n  FunctionDef: " << pTc->show();

    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element elem;
    elem.type = "function";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
	return true;
  }
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
  Repository* p_Repos;
public:
  PrintFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  bool doAction(ITokCollection*& pTc)
  {
    std::cout << "\n  FuncDef: " << pTc->show().c_str();
	return true;
  }

};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  bool doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    int len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
    std::cout << "\n";
  }
};

///////////////////////////////////////////////////////////////
// rule to detect declaration

class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end + 1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    //std::cout << "\n  -- " << tc.show();
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      //std::cout << "\n  ++ " << tc.show();
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers

      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
          se.push_back(tc[i]);
      }
      //std::cout << "\n  ** " << se.show();
      if (se.length() == 2)  // type & name, so declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show declaration

class ShowDeclaration : public IAction
{
public:
  bool doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i<tc.length(); ++i)
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    // show cleaned semiExp
    std::cout << "\n  Declaration: " << se.show();
	return true;
  }
};

///////////////////////////////////////////////////////////////
// rule to detect executable

class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end+1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end+1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }
  
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    //std::cout << "\n  -- " << tc.show();
  }
  bool doTest(ITokCollection*& pTc)
  {

    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show executable

class ShowExecutable : public IAction
{
public:
  bool doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i < tc.length(); ++i)
    {
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    }
    // show cleaned semiExp
    std::cout << "\n  Executable: " << se.show();
	return true;
  }
};

#endif
