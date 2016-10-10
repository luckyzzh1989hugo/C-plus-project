/////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp - find files matching specified patterns                    //
//               on a specified path                                       //
// ver 2.0                                                                 //
//  Language:      Visual C++ 2015                                         //
//  Platform:      Dell Inspiron 15 7000 Series, Win 10                    //
//  Application:   Project 3, CSE 687                                      //
//  Author:        Zhihao Zhang            Syracuse University             //
//                 (479)595-3948, zzhan154@syr.edu                         //
//  Source:        Jim Fawcett                                             //
/////////////////////////////////////////////////////////////////////////////

#include "FileMgr.h"
#include <iostream>

#ifdef TEST_FILEMGR

using namespace FileManager;

struct FileHandler : IFileEventHandler
{
  void execute(const std::string& fileSpec)
  {
    std::cout << "\n  --   " << fileSpec;
  }
};

struct DirHandler : IDirEventHandler
{
  void execute(const std::string& dirSpec)
  {
    std::cout << "\n  ++ " << dirSpec;
  }
};

int main()
{
  std::cout << "\n  Testing FileMgr";
  std::cout << "\n =================";

  std::string path = FileSystem::Path::getFullFileSpec("..");
  IFileMgr* pFmgr = FileMgrFactory::create(path);

  FileHandler fh;
  DirHandler dh;

  pFmgr->regForFiles(&fh);
  pFmgr->regForDirs(&dh);

  //pFmgr->addPattern("*.h");
  //pFmgr->addPattern("*.cpp");
  //pFmgr->addPattern("*.log");

  pFmgr->search();

  std::cout << "\n\n";
  return 0;
}
#endif
