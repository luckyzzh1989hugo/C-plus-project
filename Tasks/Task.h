#ifndef TASK_H
#define TASK_H
/////////////////////////////////////////////////////////////////////
//  Task.h - excute callable object in thread Pool                 //
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
declares and defines a Task class that executes a callable object on a ThreadPool thread.


Build Process:
==============
Required files
Parser.h, Parser.cpp
DependencyAnalysis.h, DependencyAnalysis.cpp
ActionsAndRules.h, ActionsAndRules.cpp,
ConfigureParser.h, ConfigureParser.cpp
 Task.h, Task.cpp, 
ThreadPool.h, ThreadPool.cpp
TypeAnalysis.h,TypeAnalysis.cpp
FileMgr.h, FileSystem.h, IFileMgr.h
FileMgr.cpp, FileSystem.cpp

Build commands
- devenv ParallelDependencyAnalysis.sln

Maintenance History:
====================
ver 1.0 : 05 Apri 16
- first release

*/
#include "../ThreadPool/ThreadPool.h"
#include "../FileMgr/FileMgr.h"
#include <map>
#include "../Parser/ActionsAndRules.h"
#include "../TypeAnalysis/TypeAnalysis.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"

/*in order to store thread id*/
struct ThreadElement
{
	ThreadElement() {};
	ThreadElement(std::unordered_set<std::string> _set) 
	{
		dependencySet = _set;
		std::stringstream ss;
		ss << std::this_thread::get_id();
		threadId = ss.str();
	}
	std::unordered_set<std::string> dependencySet;
	std::string threadId;
};
class Task
{
public:
	~Task();
	void showThreadIds();
	void setThreadNum(int _threadNum);
	void startThreadPool();
	bool waitForFinish();
	void killThread();
	void insertNull();
	void clearQueue();
	void setMergeTable(std::map<std::string, element>* table);
	void doFilesAnalysis(const std::string& path);
	void addAnalysisFileWork(const std::string& fileName);
	void insertTypeTable(std::map<std::string, element> typeTable);
	void doDependencyAnalysis(const std::string& fileName);
	std::map<std::string, element>* getMergeTable();
	void insertDependcyResult(std::string fileName, ThreadElement e);
	void pushFileName(std::string fileName);
	void doAllDependencies();
	void enableThreads();
	void setDependencyResultsTable(std::map<std::string, ThreadElement>* _dependencyResults);
	bool findFile(std::string f);
private:
	std::vector<WorkItem<std::string>> lamdaVector;
	FileManager::IFileMgr* pFmgr;
	ProcessWorkItem<std::string> threadPool;
	std::map<std::string, element>* mergeTable;
	std::map<std::string, ThreadElement>* dependencyResults;
	std::mutex mtx_;
	std::vector<std::string> fileNames;

};
struct DirHandler : FileManager::IDirEventHandler
{
	void execute(const std::string& dirSpec) {

	}
};
//------deal with each file name found
struct FileHandler : FileManager::IFileEventHandler
{

	void setTask(Task* _task)
	{
		task = _task;
	}
	void execute(const std::string& fileSpec)
	{
		task->addAnalysisFileWork(fileSpec);
	}
private:
	Task* task;
};

#endif

