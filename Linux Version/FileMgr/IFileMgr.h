/////////////////////////////////////////////////////////////////////////////
// IFileMgr.h - Interface to FileMgr                                       //
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
*createInstance();			//Generate a FileMgr Object, return a pointer
*
*Maintanence History:
*====================
*ver 1.0 - 20 Mar 2016
*- first release
*/
#ifndef IFILEMGR_H
#define IFILEMGR_H
#include <string>
class IFileMgr 
{
public:
	IFileMgr(){}
        virtual ~IFileMgr(){}
	virtual void addPattern(std::string) = 0;
	virtual void searchFiles(std::string) = 0;
	virtual std::string get() = 0;
        static IFileMgr* createInstance();
};


#endif /* IFILEMGR_H */

