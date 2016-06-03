/////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp - Collects files of a particular pattern in a Directory     //
// ver 1.0                                                                 //
// Language:    Visual C++, NetBeans 8.1                                   //
// Platform:    Thinkpad w541, Core i7, Ubuntu 14.04                       //
// Application: CSE775 Project#1, Parallel Text Searcher                   //
// Author:      Wenchang Tan, MS of CE, Syracuse University                //
//              wtan02@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
#include"./FileMgr.h"
#include "../FileSystem/FileSystem.h"
#include "../BlockingQueue/Cpp11-BlockingQueue.h"

#define FileSearchEnd "FileSearchEnd"
//-----------<Constructor>--------------
FileMgr::FileMgr()
{
    patterns.push_back("*.*");
}

//-----------<Destructor>--------------
void FileMgr::addPattern(std::string pattern)
{
    if (patterns.size() == 1 && patterns[0] == "*.*")
        patterns.pop_back();
    patterns.push_back(pattern);
    return ;
}

//-----------<Search particular pattern files>--------------
void FileMgr::searchFiles(std::string path)
{
    std::string rootPath = FileSystem::Path::getFullFileSpec(path);
    std::queue<std::string> queue;
    queue.push(rootPath);

    while (!queue.empty())
    {
        int size = queue.size();
        for (int i = 0; i < size; i++)
        {
            std::string currentPath = queue.front();
            FileSystem::Directory::setCurrentDirectory(currentPath);
            queue.pop();
            for (std::string pattern : patterns)
            {
                std::vector<std::string> files = FileSystem::Directory::getFiles(currentPath, pattern);
                for (auto& file : files)
                {
                    filesBlockingQ.enQ(currentPath + "/" + file);
                }
            }
            std::vector<std::string> dirs = FileSystem::Directory::getDirectories(currentPath);
            for (unsigned int i = 0; i < dirs.size(); i++)
            {
                std::string nextLevelPath = currentPath + "/" + dirs[i];
                queue.push(nextLevelPath);
            }
        }
    }
    filesBlockingQ.enQ(FileSearchEnd);
    return;
}
//-----------<Return a file>--------------
std::string FileMgr::get()
{
    std::string path = filesBlockingQ.deQ();
    return path;
}
//-----------<Create a FileMgr Object>--------------
IFileMgr* IFileMgr::createInstance()
{
    return new FileMgr;
}
