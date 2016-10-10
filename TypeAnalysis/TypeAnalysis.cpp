////////////////////////////////////////////////////////////////////////////////
//  TypeAnalysis.cpp - get all the types of a file                            //
//  ver 1.0                                                                   //
//  Language:      Visual C++ 2015                                            //
//  Platform:      Dell Inspiron 15 7000 Series                               //
//  Application:   Project 3, CSE 687                                         //
//  Author:        Zhihao Zhang            Syracuse University                //
//                 (479)595-3948,          zzhan154@syr.edu                   //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TypeAnalysis.h"




TypeAnalysis::TypeAnalysis(std::string _fileName)
{
	fileName = _fileName;
}
void TypeAnalysis::showTable()
{
	for (std::map<std::string, element>::iterator it = typeTable.begin(); it != typeTable.end(); ++it) {
		std::cout << it->first << "\t";
		std::cout << "[type: " << it->second.type << " ] \t";
		std::cout << "[in file:" << it->second.fileName << " ] \t";
		std::cout << "---handle by thread: " << it->second.threadId << "---- \t";
		std::cout << "\n";
	}
	std::cout << "\n\n";
}
void TypeAnalysis::doTypeAnalysis()
{

	ConfigParseToConsole configure;
	Parser* pParser = configure.buildForTypeAnalysis(&typeTable);
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
			return ;
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

std::map<std::string, element> TypeAnalysis::getTypeTable()
{
	return typeTable;
}
#ifdef zTEST_TYPEANALYSIS
int main()
{
	TypeAnalysis typeAna("TypeAnalysis.h");
	typeAna.doTypeAnalysis();
	std::map<std::string, element>result= typeAna.getTypeTable();
	for (std::map<std::string, element>::iterator it = result.begin(); it != result.end(); ++it) {
		std::cout << it->second.name << "\t" << it->second.type << "\t" << it->second.fileName << "   [handle by thred " << it->second.threadId
			<< "]\n";
	}

    return 0;
}
#endif