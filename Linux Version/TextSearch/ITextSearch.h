/////////////////////////////////////////////////////////////////////////////
// ITextSearch.h - Interface for TextSearch package                        //
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
*Maintanence History:
*====================
*ver 1.0 - 20 Mar 2016
*- first release
*/

#ifndef ITEXTSEARCH_H
#define ITEXTSEARCH_H
#include <string>
class ITextSearch
{
public:
    ITextSearch(){}
    virtual ~ITextSearch(){}

    virtual void SetThreadNum(int Num) = 0;
    virtual void searchText(std::string text) = 0;
    virtual void put(std::string file) = 0;
    virtual std::string get() = 0;
    static ITextSearch* createInstance();
};


#endif /* ITEXTSEARCH_H */

