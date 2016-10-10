////////////////////////////////////////////////////////////////////////////////////////////////
//  ParallelDependencyExecutive.cpp - demonstrate requirements of Parallel Dependency Analysis//
//  ver 1.0                                                                                   //
//  Language:      Visual C++ 2015                                                            //
//  Platform:      Dell Inspiron 15 7000 Series                                               //
//  Application:   Project 3, CSE 687                                                         //
//  Author:        Zhihao Zhang            Syracuse University                                //
//                 (479)595-3948, zzhan154@syr.edu                                            //
////////////////////////////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
provide a ParallelDependencyExecutive that finds all the compilation dependencies between files in a file collection. 
Each of the files in the collection are found in a directory tree rooted at a specified path 
and whose names match one or more specified patterns.

Build Process:
==============
Required files
Utilities.h,Utilities.cpp
ParallelDependencyAnalysis.h,ParallelDependencyAnalysis.cpp

Build commands
- devenv ParallelDependencyAnalysis.sln

Maintenance History:
====================
ver 1.0 : 05 Apri 16
- first release

*/

#include "stdafx.h"
#include <map>
#include "../Utilities/Utilities.h"
#include "../ParallelDependencyAnalysis/ParallelDependencyAnalysis.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout
			<< "\n  please enter thread numbers and root path to process on command line\n\n";
		return 1;
	}
	cout << "arguments from command line is [" <<  argv[1]<< "] and   [" << argv[2]<<"]\n";

	int threadNum = std::stoi(argv[1]);
	std::string fullPath = FileSystem::Path::getFullFileSpec(argv[2]);
	std::string message = "analyzing files rooted in [" + fullPath + " ] ";
	Utilities::StringHelper::Title(message);
	ParallelDepencyAnalysis parallelDepencyAnalysis(argv[2], threadNum);

	cout << "number of threads is  [ " << threadNum << " ] , They are :\n ";
	parallelDepencyAnalysis.showThreadIds();

	message = "after parallel type analysis, we get a big merged type table as below:";
	Utilities::StringHelper::Title(message);
	parallelDepencyAnalysis.showMergeTable();
	message = "after get the merged type table, we do the dependecies analysis using this table, the dependencies of each file are shown below";
	Utilities::StringHelper::Title(message);
	parallelDepencyAnalysis.showDependencies();
    return 0;
}

