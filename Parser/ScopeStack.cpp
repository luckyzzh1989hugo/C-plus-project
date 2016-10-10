///////////////////////////////////////////////////////////////////////////////
// ScopeStack.cpp - implements template stack holding specified element type //
// ver 2.2                                                                   //
// Language:    Visual C++, Visual Studio 2015                               //
// Platform:    Dell Inspiron 7000, Core i5, Windows 10                      //
// Application: Spring Projects, 2016                                        //
// Author:        Zhihao Zhanag, CST 4-187, Syracuse University              //
//                (479) 595-4305, zzhan154@syr.edu                           //
// Source:        Jim Fawcett                                                //
///////////////////////////////////////////////////////////////////////////////

//----< test stub >------------------------------------------------------
#include "stdafx.h"

#ifdef TEST_SCOPESTACK

#include "ScopeStack.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

struct element
{
  std::string type;
  std::string name;
  size_t startLineCount = 0;
  size_t endLineCount = 0;
  std::string show()
  {
    std::ostringstream temp;
    temp << "(";
    temp << type;
    temp << ", ";
    temp << name;
    temp << ", ";
    temp << startLineCount;
    temp << ", ";
    temp << endLineCount;
    temp << ")"; 
    return temp.str();
  }
  std::vector<element*> _children;
};

void TreeWalk(element* pItem)
{
  static size_t indentLevel = 0;
  std::cout << "\n  " << std::string(2*indentLevel, ' ') << pItem->show();
  auto iter = pItem->_children.begin();
  ++indentLevel;
  while (iter != pItem->_children.end())
  {
    TreeWalk(*iter);
    ++iter;
  }
  --indentLevel;
}

int main()
{
  std::cout << "\n  Testing Scope Stack";
  std::cout << "\n =====================\n";

  std::cout << "\n  pushing items onto ScopeStack instance after adding startLineCount";
  std::cout << "\n --------------------------------------------------------------------";

  ScopeStack<element*> testStack;
  element* pItem = new element;

  pItem->type = "function";
  pItem->name = "fun1";
  pItem->startLineCount = 33;
  testStack.push(pItem);

  pItem = new element;
  pItem->type = "if";
  pItem->name = "";
  pItem->startLineCount = 38;
  (testStack.top()->_children).push_back(pItem);
  testStack.push(pItem);

  pItem = new element;
  pItem->type = "for";
  pItem->name = "";
  pItem->startLineCount = 40;
  (testStack.top()->_children).push_back(pItem);
  testStack.push(pItem);


  showStack<element>(testStack, false);
  std::cout << std::endl;

}

#endif
