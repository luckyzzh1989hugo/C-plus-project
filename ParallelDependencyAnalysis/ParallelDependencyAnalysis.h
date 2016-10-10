#ifndef PARALLELDEPENDEENCYANALYSIS
#define PARALLELDEPENDEENCYANALYSIS
////////////////////////////////////////////////////////////////////////////////
//  ParallelDependencyAnalysis.h - Analyze dependcy of a file collection      //
//  ver 1.0                                                                   //
//  Language:      Visual C++ 2015                                            //
//  Platform:      Dell Inspiron 15 7000 Series                               //
//  Application:   Project 3, CSE 687                                         //
//  Author:        Zhihao Zhang            Syracuse University                //
//                 (479)595-3948,          zzhan154@syr.edu                   //
////////////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
finds compilation dependencies between all of the files in a specified 
file collection using the asynchronous processing facilities provided by the Task package.


Build Process:
==============
Required files
ActionsAndRules.h, ActionsAndRules.cpp



Build commands
- devenv ParallelDependencyAnalysis.sln

Maintenance History:
====================
ver 1.0 : 05 Apri 16
- first release

*/
#include "../Tasks/Task.h"
#include "../Parser/ActionsAndRules.h"

class ParallelDepencyAnalysis
{
public:
	ParallelDepencyAnalysis(std::string path,  int threadNum);
	void showMergeTable();
	void showDependencies();
	void showThreadIds();
	void setThreadNums(int threadNums);
private:
	std::map<std::string, element> mergeTable;
	std::map<std::string, ThreadElement> dependencyResultsMap;
	Task task;
};


#endif
