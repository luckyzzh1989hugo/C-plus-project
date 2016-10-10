////////////////////////////////////////////////////////////////////////////////
//  ParallelDependencyAnalysis.cpp - Analyze dependcy of a file collection    //
//  ver 1.0                                                                   //
//  Language:      Visual C++ 2015                                            //
//  Platform:      Dell Inspiron 15 7000 Series                               //
//  Application:   Project 3, CSE 687                                         //
//  Author:        Zhihao Zhang            Syracuse University                //
//                 (479)595-3948,           zzhan154@syr.edu                  //
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ParallelDependencyAnalysis.h"
#include <sstream>
#include "../Utilities/Utilities.h"



ParallelDepencyAnalysis::ParallelDepencyAnalysis(std::string path,int threadNum)
{
	task.setThreadNum(threadNum);
	task.startThreadPool();
	task.setDependencyResultsTable(&dependencyResultsMap);
	task.setMergeTable(&mergeTable);
	task.doFilesAnalysis(path);	
	task.insertNull();
	task.waitForFinish();

	task.enableThreads();
	task.doAllDependencies();
	task.insertNull();
	task.waitForFinish();

	task.killThread();

}

void ParallelDepencyAnalysis::showMergeTable()
{
	for (std::map<std::string, element>::iterator it = mergeTable.begin(); it != mergeTable.end(); ++it) {
		std::cout << it->first<<"\t";
		std::cout << "[type: " << it->second.type << " ] \t";
		std::cout << "[in file:" << it->second.fileName << " ] \t";
		std::cout << "[handle by thread: " << it->second.threadId << "] \t";
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

void ParallelDepencyAnalysis::showDependencies()
{
	for (std::map<std::string, ThreadElement>::iterator it = dependencyResultsMap.begin(); it != dependencyResultsMap.end(); ++it) {
		std::stringstream ss;
		ss << "\n [handled by thread: " << it->second.threadId << "] " << it->first << "  depends on : ";

		Utilities::StringHelper::title(ss.str());
		for each(std::string file in it->second.dependencySet)
		{
			std::cout << file << "\n";
		}
		std::cout << "-----------------------------------------------------------------------------------------------";
	}
}

void ParallelDepencyAnalysis::showThreadIds()
{
	task.showThreadIds();
}

void ParallelDepencyAnalysis::setThreadNums(int threadNums)
{
}

#ifdef zTEST_PARALLELDEPENDENCYANALYSIS
int main()
{
	
	ParallelDepencyAnalysis parallelDepencyAnalysis("..",4);

	parallelDepencyAnalysis.showMergeTable();
	parallelDepencyAnalysis.showDependencies();

	return 0;
}
#endif