/////////////////////////////////////////////////////////////////////////////
// FileMgr.h - Declaration of the CFileMgr							       //
// ver 1.0                                                                 //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    Thinkpad w541, Core i7, Windows 10 Pro                     //
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
*addPattern();						//add perticular patterns for file search
*searchFiles();                   	//find particular files and then save in the map
*get(); 							//return a file result
*
*Maintanence History:
*====================
*ver 1.0 - 20 Mar 2016
*- first release
*/

#pragma once
#include "resource.h"       // main symbols



#include "FileMgrCom_i.h"
#include<string>
#include<vector>
#include<queue>
#include"../BlockingQueue/Cpp11-BlockingQueue.h"
#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CFileMgr

class ATL_NO_VTABLE CFileMgr :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFileMgr, &CLSID_FileMgr>,
	public IDispatchImpl<IFileMgr, &IID_IFileMgr, &LIBID_FileMgrComLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CFileMgr()
	{
		patterns.push_back("*.*");
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FILEMGR)


BEGIN_COM_MAP(CFileMgr)
	COM_INTERFACE_ENTRY(IFileMgr)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(addPattern)(BSTR pattern);
	STDMETHOD(searchFiles)(BSTR path_);
	STDMETHOD(get)(BSTR* file);
private:
	BlockingQueue<std::string> filesBlockingQ;
	std::vector<std::string> patterns;
	std::string& BstrToStdString(const BSTR bstr, std::string& dst, int cp = CP_UTF8);
};

OBJECT_ENTRY_AUTO(__uuidof(FileMgr), CFileMgr)
