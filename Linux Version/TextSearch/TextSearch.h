/////////////////////////////////////////////////////////////////////////////
// TextSearch.h - Collects files contains particular text in a set of files//
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
*Provides support for multi-threading text search in a set of files
*
*Public Interfaces:
*==================
*put();                                         //add files to inputQueue
*SetThreadNum();				//set number of threads to search
*searchFiles();                                 //entry of package
*get();                                         //return a file result 
*
*Required Files:
*===============
*TextSearch.cpp TextSearch.h ITextSearch.h
*
*Maintanence History:
*====================
*ver 1.0 - 20 Mar 2016
*- first release
*/

#pragma once

#include "ITextSearch.h"
#include "../BlockingQueue/Cpp11-BlockingQueue.h"
#include <atomic>
#include <thread>
#include <unordered_map>
#include <condition_variable>

#define Active true;
#define Sleep false;

class TextSearch : public ITextSearch
{
public:
    TextSearch();
    virtual ~TextSearch();

    void SetThreadNum(int Num);
    void searchText(std::string text);
    void put(std::string file);
    std::string get();

private:

    void doSearch();
    bool searchInFile(std::string filename);
    void checkSearchStatus();
    BlockingQueue<std::string> inputQueue;
    BlockingQueue<std::string> resultQueue;
    std::atomic<bool> exitSearch;
    std::string keyWords;
    std::unordered_map<std::thread::id, bool> threadPool;
    std::mutex ioLock;
    std::mutex mtx_;
    std::condition_variable cv_;
    static bool Allocated;
    static bool searching;
};

