#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H
////////////////////////////////////////////////////////////////////////////////
//  TypeAnalysis.h - get all the types of a file                              //
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
Finds all the types defined in each of a collection of C++ source files. It does this by building 
rules to detect type definitions: classes, structs, enums, typedefs, and aliases, and capture their 
fully qualified names and files where they are defined.


Build Process:
==============
Required files
ConfigureParser.h, ConfigureParser.cpp
TypeAnalysis.h,TypeAnalysis.cpp



Build commands
- devenv ParallelDependencyAnalysis.sln

Maintenance History:
====================
ver 1.0 : 05 Apri 16
- first release

*/
#include <map>
#include "..\Parser\ConfigureParser.h"


class TypeAnalysis
{
public:
	TypeAnalysis(std::string _fileName);
	void doTypeAnalysis();
	std::map<std::string, element>getTypeTable();
	void showTable();
private: 
	std::string fileName;
	std::map<std::string, element> typeTable;


};
#endif