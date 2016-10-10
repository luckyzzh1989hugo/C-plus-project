/////////////////////////////////////////////////////////////////////
//  Task.cpp - excute callable object in thread Pool               //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron 15 7000 Series                    //
//  Application:   Project 3, CSE 687                              //
//  Author:        Zhihao Zhang            Syracuse University     //
//                 (479)595-3948, zzhan154@syr.edu                 //
/////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "../TypeAnalysis/TypeAnalysis.h"
#include "Task.h"
#include "../FileMgr/FileMgr.h"
#include "../Parser/Parser.h"
#include "../DependencyAnalysis/DependencyAnalysis.h"



//-----initialize thread numbers of thread pool
void Task::setThreadNum(int threadNum)
{
	threadPool.setThreadsNum(threadNum);

}
Task::~Task()
{
	delete pFmgr;
}
void Task::showThreadIds()
{
	threadPool.showThreadId();
}
//---start threadPool
void Task::startThreadPool()
{
	threadPool.start();
	threadPool.enableThreads();
}
/*check and wait for all the thread done their job*/
bool Task::waitForFinish()
{
	while (threadPool.threadsDoing()) {  }
	clearQueue();
	return true;

}
//-------kill threads
void Task::killThread()
{
	threadPool.end();
	threadPool.wait();
}

//indicate thread to rest
void Task::insertNull()
{
	threadPool.doWork(nullptr);
}
/*clear thread pool's working Queue*/
void Task::clearQueue()
{
	threadPool.clearWorkQueue();
}
void Task::setMergeTable(std::map<std::string, element>* table)
{
	mergeTable = table;
}
/* get files*/;
void Task::doFilesAnalysis(const std::string& path)
{
	std::string fullPath = FileSystem::Path::getFullFileSpec(path);
	FileManager::IFileMgr* pFmgr = FileManager::FileMgrFactory::create(fullPath);

	FileHandler fh;
	fh.setTask(this);

	DirHandler dh;

	pFmgr->regForFiles(&fh);
	pFmgr->regForDirs(&dh);
	pFmgr->addPattern("*.h");
	pFmgr->addPattern("*.cpp");
	pFmgr->search();

}
//---ask typeAnalysis to create type table
void Task::addAnalysisFileWork(const std::string & fileName)
{
	WorkItem<std::string> wi1 = [fileName,this]() {
		TypeAnalysis typeAnalysis(fileName);
		typeAnalysis.doTypeAnalysis();
		{
			//std::lock_guard<std::mutex> l(mtx_);
			this->pushFileName(fileName);
			this->insertTypeTable(typeAnalysis.getTypeTable());
		}
		return "";

	};
	threadPool.doWork(wi1);
	
}
//------Merge typpe table
void Task::insertTypeTable(std::map<std::string, element> typeTable)
{
	mergeTable->insert(typeTable.begin(), typeTable.end());
}
//---------lamda for dependency analysis
void Task::doDependencyAnalysis(const std::string & fileName)
{
	WorkItem<std::string> wi2 = [fileName, this]() {
		//std::cout << "\n  working on thread " << std::this_thread::get_id();
		 DependencyAnalysis dependencyAnalysis(this->getMergeTable(),fileName);
		 dependencyAnalysis.doDependencyAnalysis();
		{
			std::lock_guard<std::mutex> l(mtx_);
			ThreadElement te(dependencyAnalysis.getDependncyTable());
			if (fileName.find(".h")!=-1)
			{
				std::string fCpp = fileName;
				fCpp.erase(fCpp.end()-1);
				fCpp.append("cpp");
				if (findFile(fCpp))te.dependencySet.insert(fCpp);
			}
			this->insertDependcyResult(fileName,te);
		}
		return "";

	};
	threadPool.doWork(wi2);
}
std::map<std::string, element>* Task::getMergeTable()
{
	return mergeTable;
}
/*insert results to a result map*/
void Task::insertDependcyResult(std::string fileName, ThreadElement e)
{
	(*dependencyResults)[fileName] = e;
}
void Task::pushFileName(std::string fileName)
{
	fileNames.push_back(fileName);
}
void Task::doAllDependencies()
{
	size_t len = fileNames.size();
	for (size_t i = 0; i < len; i++)
	{
		doDependencyAnalysis(fileNames[i]);
	}
}
void Task::enableThreads()
{
	threadPool.enableThreads();
}
void Task::setDependencyResultsTable(std::map<std::string, ThreadElement>* _dependencyResults)
{
	dependencyResults = _dependencyResults;
}
bool Task::findFile(std::string f)
{
	for each(std::string file in fileNames)
	{
		if (file.compare(f) == 0)return true;
	}
	return false;
}
#ifdef zTEST_TASK
int main()
{

	int a = 0;
	Task task();
	task.setThreadNum();
	task.startThreadPool();
	std::map<std::string, element> mergeTable;
	std::map<std::string, ThreadElement> dependencyResultsMap;
	task.setDependencyResultsTable(&dependencyResultsMap);
	task.setMergeTable(&mergeTable);
	task.doFilesAnalysis("..");
	task.insertNull();
	task.waitForFinish();
	task.enableThreads();
	task.doAllDependencies();
	task.insertNull();
	task.waitForFinish();
	task.killThread();
	
	for (std::map<std::string, ThreadElement>::iterator it = dependencyResultsMap.begin(); it != dependencyResultsMap.end(); ++it) {
		std::cout << "\n [handled by thread: "<<it->second.threadId<<"] files "<<it->first<<"depends on are : \n";
		for each(std::string file in it->second.dependencySet)
		{
			std::cout << file <<"\n";
		}
	}

}
#endif
