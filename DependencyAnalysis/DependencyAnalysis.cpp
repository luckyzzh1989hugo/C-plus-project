//  DependencyAnalysis.cpp - Analyze dependcy of a file              //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron 15 7000 Series                    //
//  Application:   Project 3, CSE 687                              //
//  Author:        Zhihao Zhang            Syracuse University     //
//                 (479)595-3948, zzhan154@syr.edu                 //
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DependencyAnalysis.h"
#include "../Parser/ConfigureParser.h"




DependencyAnalysis::DependencyAnalysis(std::map<std::string, element>* _mergeMap, std::string _fileName)
{
	fileName = _fileName;
	mergeMap = _mergeMap;

}

std::unordered_set<std::string> DependencyAnalysis::getDependncyTable()
{
	return dependencyTable;
}
void DependencyAnalysis::showDependencyTable()
{
	for each(std::string file in dependencyTable)
	{
		std::cout << file << "\n";
	}
}
void DependencyAnalysis::doDependencyAnalysis()
{
	ConfigParseToConsole configure;
	Parser* pParser = configure.buildForDependenceAnalysis(mergeMap, &dependencyTable);
	try
	{
		if (pParser)
		{
			if (!configure.Attach(fileName))
			{
				std::cout << "\n  could not open file " << fileName << std::endl;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return;
		}
		// now that parser is built, use it

		while (pParser->next())
			pParser->parse();

	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
}
#ifdef TEST_DEPENDENCYANALYSIS
int main()
{

	return 0;
}
#endif
