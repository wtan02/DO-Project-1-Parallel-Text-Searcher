/////////////////////////////////////////////////////////////////////////////
// TextSearch.h - Implementation of CTextSearch							   //
// ver 1.0                                                                 //
// Language:    Visual C++, NetBeans 8.1                                   //
// Platform:    Thinkpad w541, Core i7, Ubuntu 14.04                       //
// Application: CSE775 Project#1, Parallel Text Searcher                   //
// Author:      Wenchang Tan, MS of CE, Syracuse University                //
//              wtan02@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "TextSearch.h"
#include "../FileSystem/FileSystem.h"
#include <thread>
#define FileSearchEnd "FileSearchEnd"
#define TextSearchEnd "TextSearchEnd"
#define Active true;
#define Sleep false;
// CTextSearch
bool CTextSearch::Allocated = false;
bool CTextSearch::searching = false;
//-----------<Input files to inputQueue>--------------
STDMETHODIMP CTextSearch::put(BSTR filePath_)
{
	std::string filePath;
	BstrToStdString(filePath_, filePath);
	CComBSTR FileSearchEndSig("FileSearchEnd");
	if (filePath == FileSearchEnd)
	{
		for (size_t threadNum = 0; threadNum < threadPool.size(); threadNum++) 
			inputQueue.enQ(FileSearchEnd);
	}
	else 
	{
		inputQueue.enQ(filePath);
	}
	return S_OK;
}

//-----------<Return a file>--------------
STDMETHODIMP CTextSearch::get(BSTR* result_)
{
	std::string resultFilePath = resultQueue.deQ();;
	CComBSTR bstr(resultFilePath.c_str());
	*result_ = bstr;
	bstr.Detach();
	return S_OK;
}

std::string& CTextSearch::BstrToStdString(const BSTR bstr, std::string& dst, int cp)
{
	if (!bstr)
	{
		// define NULL functionality.
		dst.clear();
		return dst;
	}
	int res = WideCharToMultiByte(cp, 0, bstr, -1, NULL, 0, NULL, NULL);
	if (res > 0)
	{
		dst.resize(res);
		WideCharToMultiByte(cp, 0, bstr, -1, &dst[0], res, NULL, NULL);
		dst = dst.substr(0, dst.size() - 1);
	}
	else
	{    // no content. clear target
		dst.clear();
	}
	
	return dst;
}

//-----------<Get number of working thread>--------------
STDMETHODIMP CTextSearch::SetThreadNum(LONG Num)
{
	if (!Allocated)
	{
		Allocated = true;
		for (long i = 0; i < Num; i++)
		{
			
			std::thread childThread([&]()
			{
				doSearch();
			});
			std::lock_guard<std::mutex> l(ioLock);
			std::cout << "\n  Text search thread " << childThread .get_id() <<" was created"<< std::endl;
			if (threadPool.find(childThread.get_id()) == threadPool.end()) {
				threadPool[childThread.get_id()] = Active;
			}
			inputQueue.enQ(FileSearchEnd);
			childThread.detach();
		}
		
	}
	else 
	{
		std::lock_guard<std::mutex> l(ioLock);
		std::cout << "\n  Can't set number of threads again!" << std::endl;
	}
	return S_OK;
}

//-----------<thread proxcy>--------------
void CTextSearch::doSearch()
{
	while (!exitSearch)
	{
		std::string file = inputQueue.deQ();
		if (file == FileSearchEnd)
		{
			threadPool[std::this_thread::get_id()] = Sleep;
			std::unique_lock<std::mutex> l(mtx_);
			while (!threadPool[std::this_thread::get_id()]) 
				cv_.wait(l, [this]() { return threadPool[std::this_thread::get_id()]; });
		}
		else
		{
			if (searchFile(file))
				resultQueue.enQ(file);
		}
	}
}

//-----------<Search particular text in files>--------------
bool CTextSearch::searchFile(std::string filename)
{
	FileSystem::File sourcefile(filename);
	sourcefile.open(FileSystem::File::in);

	if (!sourcefile.isGood())
	{
		std::lock_guard<std::mutex> l(ioLock);
		std::cout << "\n  Can't open file " << sourcefile.name();
		std::cout << "\n  Here's what the program can't find:\n  " << FileSystem::Path::getFullFileSpec(sourcefile.name());
		return false;
	}
	while (sourcefile.isGood())
	{
		if (!sourcefile.isGood())
			break;
		std::string buffer = sourcefile.getLine();
		unsigned int loc = buffer.find(keyWords);
		if (loc != std::string::npos)
		{
			sourcefile.close();	
			return true;
		}
	}
	sourcefile.close();
	return false;
}

//-----------<Entry of package>--------------
STDMETHODIMP CTextSearch::searchText(BSTR text_)
{
	if (!searching)
	{
		searching = true;
		std::string text;
		BstrToStdString(text_, text);
		keyWords = text;

		while (true)
		{
			bool canBreak = true;
			for (auto& thread : threadPool) 
			{
				if (thread.second == true)
					canBreak = false;
			}
			if (canBreak == true)
				break;
		}

		for (auto& thread : threadPool)
			thread.second = Active;
		cv_.notify_all();

		//checkstatus
		std::thread checkStatusThread([&]()
		{
			checkSearchStatus();
		});
		checkStatusThread.detach();
	}
	else
	{
		std::lock_guard<std::mutex> l(ioLock);
		std::cout << "\n  Searching " + keyWords + " , please try again later!" << std::endl;
	}

	return S_OK;
}

//-----------<Check threading status>--------------
void CTextSearch::checkSearchStatus()
{
	while (inputQueue.size() > 0) 
	{
		;
	}
	resultQueue.enQ(TextSearchEnd);
}