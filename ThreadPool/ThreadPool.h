#ifndef THREADPOOL_H
#define THREADPOOL_H
/////////////////////////////////////////////////////////////////////
//  ThreadPool.h - provide a container for threads                 //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron 15 7000 Series                    //
//  Application:   Project 3, CSE 687                              //
//  Author:        Zhihao Zhang            Syracuse University     //
//                 (479)595-3948, zzhan154@syr.edu                 //
//  Source:        Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
declares and defines a ThreadPool class that executes enqueued work items asynchronously


Build Process:
==============
Required files
 ThreadPool.h, ThreadPool.cpp, BlockingQueue.h
 Utilities.h,Utilities.cpp

Build commands
- devenv ParallelDependencyAnalysis.sln

Maintenance History:
====================
ver 1.0 : 05 Apri 16
- first release

*/

#include <thread>
#include <functional>
#include <vector>
#include <map>
#include "BlockingQueue.h"

template<typename Result>
using WorkItem = std::function<Result()>;

///////////////////////////////////////////////////////////////////////
// class to process work items
// - each work item is processed sequentially on a single child thread

template<typename Result>
class ProcessWorkItem
{
public:
	void start();
	void doWork(WorkItem<Result> pWi);
	Result result();
	~ProcessWorkItem();
	void wait();
	void enableThreads();
	void end(); 
	bool threadsDoing();
	void clearWorkQueue();
	void setThreadsNum(int _threadNum);
	void showThreadId();
private:
	int threadNum = 3;   //set threadNums
	std::map<std::thread::id,bool> threadPoolFlags;
	std::vector<std::thread*> threadPool;
	BlockingQueue<WorkItem<Result>> _workItemQueue;
	BlockingQueue<Result> _resultsQueue;
	bool threadsAlive; //flag that indicate threads to end
};


// kill threads
template<typename Result>
void ProcessWorkItem<Result>::end()
{
	threadsAlive = false;
}

template<typename Result>
bool ProcessWorkItem<Result>::threadsDoing()
{
	for (std::map<std::thread::id, bool>::iterator it = threadPoolFlags.begin(); it != threadPoolFlags.end(); ++it) {
		if (it->second)return true;
	}
	return false;
}

template<typename Result>
inline void ProcessWorkItem<Result>::clearWorkQueue()
{
	_workItemQueue.clear();
}

template<typename Result>
inline void ProcessWorkItem<Result>::setThreadsNum(int _threadNum)
{
	threadNum = _threadNum;
}

template<typename Result>
inline void ProcessWorkItem<Result>::showThreadId()
{
	for (std::map<std::thread::id, bool>::iterator it = threadPoolFlags.begin(); it != threadPoolFlags.end(); ++it) {
		std::cout << "thread [ " << it->first << " ]\n ";
	}
}

//------enable threads
template<typename Result>
void ProcessWorkItem<Result>::enableThreads()
{
	for (std::map<std::thread::id, bool>::iterator it = threadPoolFlags.begin(); it != threadPoolFlags.end(); ++it) {
		it->second = true;
	}
}
//let threads join main threads
template<typename Result>
void ProcessWorkItem<Result>::wait()
{
	size_t len = threadPool.size();
	for (size_t i = 0; i < len; i++)
	{
		threadPool[i]->join();
	}
}
//----< enqueue work item >--------------------------------------------

template<typename Result>
void ProcessWorkItem<Result>::doWork(WorkItem<Result> pWi)
{
	_workItemQueue.enQ(pWi);
}
//----< retrieve results with blocking call >--------------------------

template<typename Result>
Result ProcessWorkItem<Result>::result()
{
	return _resultsQueue.deQ();
}
//----< start child thread that dequeus work items >-------------------

template<typename Result>
void ProcessWorkItem<Result>::start()
{
	threadsAlive = true;
	for (int i = 0; i < threadNum; i++)
	{
		std::function<void()> threadProc =
			[&]() {
			while (threadsAlive)
			{
				
				if (threadPoolFlags.find(std::this_thread::get_id())== threadPoolFlags.end()||
					!threadPoolFlags[std::this_thread::get_id()])continue;
				WorkItem<Result> pWi = _workItemQueue.deQ();
				if (pWi == nullptr)
				{
					_workItemQueue.enQ(nullptr);
					threadPoolFlags[std::this_thread::get_id()] = false;
					continue;
				}
				Result result = pWi();
				//_resultsQueue.enQ(result);
				//std::cout << "thread id: "<< std::this_thread::get_id() << "item" <<result << "\n";
			}
		};
		std::thread* _pThread=new std::thread(threadProc);
		threadPoolFlags[_pThread->get_id()]=false;
		threadPool.push_back(&(*_pThread));
	}

}
//----< clean up heap >------------------------------------------------

template<typename Result>
ProcessWorkItem<Result>::~ProcessWorkItem()
{
	for (size_t i = 0; i < threadPool.size(); i++)
	{
		delete threadPool[i];
	}
}
#endif


