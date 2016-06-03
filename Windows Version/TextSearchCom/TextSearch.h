/////////////////////////////////////////////////////////////////////////////
// TextSearch.h - Declaration of the CTextSearch						   //
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
*SetThreadNum();								//set number of threads to search
*searchFiles();                                 //entry of package
*get();                                         //return a file result
*
*Maintanence History:
*====================
*ver 1.0 - 20 Mar 2016
*- first release
*/
// TextSearch.h : Declaration of the CTextSearch

#pragma once
#include "resource.h"       // main symbols



#include "TextSearchCom_i.h"
#include "../BlockingQueue/Cpp11-BlockingQueue.h"
#include <atomic>
#include <unordered_map>
#include <condition_variable>
#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif
#define Active true;
#define Sleep false;
using namespace ATL;


// CTextSearch

class ATL_NO_VTABLE CTextSearch :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTextSearch, &CLSID_TextSearch>,
	public IDispatchImpl<ITextSearch, &IID_ITextSearch, &LIBID_TextSearchComLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTextSearch()
	{
		exitSearch = false;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TEXTSEARCH)


BEGIN_COM_MAP(CTextSearch)
	COM_INTERFACE_ENTRY(ITextSearch)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		for (auto& thread : threadPool)
		{
			thread.second = Active;
		}
		exitSearch = true;
		cv_.notify_all();
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

public:



	STDMETHOD(put)(BSTR filePath);
	STDMETHOD(get)(BSTR* result_);
private:
	std::string& BstrToStdString(const BSTR bstr, std::string& dst, int cp = CP_UTF8);
	void doSearch();
	bool searchFile(std::string filename);
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
public:
	STDMETHOD(SetThreadNum)(LONG Num);
	STDMETHOD(searchText)(BSTR text);
};

OBJECT_ENTRY_AUTO(__uuidof(TextSearch), CTextSearch)
