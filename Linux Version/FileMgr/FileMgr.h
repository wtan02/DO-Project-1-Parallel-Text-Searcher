/////////////////////////////////////////////////////////////////////////////
// FileMgr.h - Collects files of a particular pattern in a Directory       //
// ver 1.0                                                                 //
// Language:    Visual C++, NetBeans 8.1                                   //
// Platform:    Thinkpad w541, Core i7, Ubuntu 14.04                       //
// Application: CSE775 Project#1, Parallel Text Searcher                   //
// Author:      Wenchang Tan, MS of CE, Syracuse University                //
//              wtan02@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
/*
*Package Operations:
*===================
*Provides support for collecting files of a particular set of patterns
*in a Directory
*
*Public Interfaces:
*==================
*addPattern();				//add perticular patterns for file search
*searchFiles();                   	//find particular files and then save in the map
*get(); 				//return a file result 
*
*Required Files:
*===============
*FileMgr.cpp FileMgr.h 
*
*Maintanence History:
*====================
*ver 1.0 - 20 Mar 2016
*- first release
*/
#include<string>
#include"../BlockingQueue/Cpp11-BlockingQueue.h"
#include "IFileMgr.h"
class FileMgr : public IFileMgr
{
public:
    FileMgr();
    virtual ~FileMgr(){}
    void addPattern(std::string);
    void searchFiles(std::string);
    std::string get();

private:
    BlockingQueue<std::string> filesBlockingQ;
    std::vector<std::string> patterns;
};








#ifndef FILEMGR_H
#define FILEMGR_H

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* FILEMGR_H */

