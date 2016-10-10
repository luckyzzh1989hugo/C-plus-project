/////////////////////////////////////////////////////////////////////
//  ThreadPool.h - provide a container for threads                 //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron 15 7000 Series                    //
//  Application:   Project 3, CSE 687                              //
//  Author:        Zhihao Zhang            Syracuse University     //
//                 (479)595-3948, zzhan154@syr.edu                 //
/////////////////////////////////////////////////////////////////////
/*
* A single child thread processes work items equeued by main thread
*/
#include "stdafx.h"
#include <string>
#include <iostream>
#include <conio.h>
#include "ThreadPool.h"
#include "../Utilities/Utilities.h"
#include <vector>

//----< demonstrate ProcessWorkItem class >----------------------------



#ifdef zTEST_THREADPOOL
using Util = Utilities::StringHelper;
using WorkResult = std::string;
WorkItem<WorkResult> creatLamda(int i)
{
	WorkItem<WorkResult> wi1 = [i]() {
		//std::cout << "\n  working on thread " << std::this_thread::get_id();

		return std::to_string(i);
	};
	return wi1;
}
int main()
{	
	Util::Title("Enqueued Work Items");

	std::cout << "\n  main thread id = " << std::this_thread::get_id();

	ProcessWorkItem<WorkResult> processor;
	processor.start();
	std::vector<WorkItem<WorkResult>> lamdaVector;
	 //define 1st work item
	processor.enableThreads();
	for (int i = 0; i < 100; i++)
	{
		WorkItem<WorkResult> wi1 = creatLamda(i);
		processor.doWork(wi1);
	}

	processor.doWork(nullptr);

	while (processor.threadsDoing());
	processor.end();
	

	processor.wait();

	std::cout << "\n\n";
}
#endif