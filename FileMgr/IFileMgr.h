#ifndef IFILEEVENTHANDLER_H
#define IFILEEVENTHANDLER_H
/////////////////////////////////////////////////////////////////////
//  IFileMgr.h - package for the FileMgr interface                    //
//  ver 1.0                                                        //
// Language:    Visual C++, Visual Studio 2015                     //
// Platform:    Dell Inspiron 7000, Core i5, Windows 10            //
// Application: Spring Projects, 2016                              //
// Author:      Zhihao Zhang,  Syracuse University                 //
//              (479) 595-4305, zzhan154@syr.edu                   //
// Source:		Jim Fawcett                                        //
/////////////////////////////////////////////////////////////////////
/*



Build commands
- devenv ParallelDependencyAnalysis.sln

Module Purpose:
===============
An interface for adding IFileEventHandler and IDirEventHandler

Maintenance History:
====================
ver 1.0 : 05 Apr 16
- first release
*/

#include <string>

namespace FileManager
{
  struct IFileEventHandler
  {
    virtual void execute(const std::string& fileSpec) = 0;
  };

  struct IDirEventHandler
  {
    virtual void execute(const std::string& dirSpec) = 0;
  };

  struct IFileMgr
  {
    virtual ~IFileMgr() {}
    virtual void addPattern(const std::string& patt) = 0;
    virtual void search() = 0;
    virtual void find(const std::string& path) = 0;
    virtual void regForFiles(IFileEventHandler* pEventHandler) = 0;
    virtual void regForDirs(IDirEventHandler* pEventHandler) = 0;
  };

  struct FileMgrFactory
  {
    static IFileMgr* create(const std::string& path);
  };
}
#endif