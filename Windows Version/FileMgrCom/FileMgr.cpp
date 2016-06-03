
/////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp - Implementation of CFileMgr							       //
// ver 1.0                                                                 //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    Thinkpad w541, Core i7, Windows 10 Pro                     //
// Application: CSE775 Project#1, Parallel Text Searcher                   //
// Author:      Wenchang Tan, MS of CE, Syracuse University                //
//              wtan02@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp : Implementation of CFileMgr
#include "stdafx.h"
#include "FileMgr.h"
#include "../FileSystem/FileSystem.h"
#include "../BlockingQueue/Cpp11-BlockingQueue.h"
// CFileMgr
#define FileSearchEnd "FileSearchEnd"


STDMETHODIMP CFileMgr::addPattern(BSTR pattern_)
{
	std::string pattern;
	BstrToStdString(pattern_, pattern);
	
	if (patterns.size() == 1 && patterns[0] == "*.*")
		patterns.pop_back();
	patterns.push_back(pattern);
	return S_OK;
}
//-----------<Search particular pattern files>--------------
STDMETHODIMP CFileMgr::searchFiles(BSTR path_)
{
	std::string path;
	BstrToStdString(path_, path);
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
					filesBlockingQ.enQ(currentPath + "\\" + file);
				}
			}
			std::vector<std::string> dirs = FileSystem::Directory::getDirectories(currentPath);
			for (unsigned int i = 2; i < dirs.size(); i++)
			{
				std::string nextLevelPath = currentPath + "\\" + dirs[i];
				queue.push(nextLevelPath);
			}
		}
	}
	
	filesBlockingQ.enQ(FileSearchEnd);
	return S_OK;
}
//-----------<Return a file>--------------
STDMETHODIMP CFileMgr::get(BSTR* file)
{
	std::string path = filesBlockingQ.deQ();
	CComBSTR bstr(path.c_str());
	*file = bstr;
	bstr.Detach();
	return S_OK;
}
//-----------<transformer helper function>--------------
std::string& CFileMgr::BstrToStdString(const BSTR bstr, std::string& dst, int cp)
{
	if (!bstr)
	{
		// define NULL functionality.
		dst.clear();
		return dst;
	}

	// request content length in single-chars through a terminating
	//  nullchar in the BSTR. note: BSTR's support imbedded nullchars,
	//  so this will only convert through the first nullchar.
	int res = WideCharToMultiByte(cp, 0, bstr, -1, NULL, 0, NULL, NULL);
	if (res > 0)
	{
		dst.resize(res);
		WideCharToMultiByte(cp, 0, bstr, -1, &dst[0], res, NULL, NULL);
	}
	else
	{    // no content. clear target
		dst.clear();
	}
	return dst;
}