#ifndef DEPENDCYANALYSIS_H
#define DEPENDCYANALYSIS_H
/////////////////////////////////////////////////////////////////////
//  DependencyAnalysis.h - Analyze dependcy of a file              //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron 15 7000 Series                    //
//  Application:   Project 3, CSE 687                              //
//  Author:        Zhihao Zhang            Syracuse University     //
//                 (479)595-3948, zzhan154@syr.edu                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a DependencyAnalysis class. It can anaylze the dependcy of a file with
a the help of a big type table, which can be pass in this class


Build Process:
==============
Required files
ActionsAndRules.h,ActionsAndRules.cpp

Build commands
- devenv ParallelDependencyAnalysis.sln

Maintenance History:
====================
ver 1.0 : 05 Apri 16
- first release

*/

#include <unordered_set>
#include <map>
#include "../Parser/ActionsAndRules.h"
class DependencyAnalysis
{
public: 
	DependencyAnalysis(std::map<std::string, element>* mergeMap, std::string fileName);
	std::unordered_set<std::string> getDependncyTable();
	void doDependencyAnalysis();
	void showDependencyTable();
private:
	std::string fileName;
	std::unordered_set<std::string> dependencyTable;
	std::map<std::string, element>* mergeMap;
};


#endif
