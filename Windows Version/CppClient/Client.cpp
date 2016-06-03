/////////////////////////////////////////////////////////////////////////////
// Client.cpp - Demonstrate Parallel Text Search Component				   //
//			    CSE775 Distributed Object								   //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    Thinkpad w541, Core i7, Windows 10 pro	                   //
// Application: CSE775 Project#1, Parallel Text Search Component           //
// Author:      Wenchang Tan, MS of CE, Syracuse University                //
//              wtan02@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* Provides demostration for parallel text search of a particular set of patterns 
* and a keyword in a Directory using COM technology
*
* Rebuilding projects in this solution:
* =====================================
* 1. Make sure you are running Visual Studio as administrator.
* 2. Right-click on idl file and select compile.
* 3. Rebuild FileMgrCom and TextSearchCom and register using Tools > Register Component.
*    This step assumes you've added Register Component to your External Tools.
*    Otherwise you have to navigate to the project debug directory and run
*    regsvr32 FileMgrCom.dll and TextSearchCom.dll.
* 4. The CppClient doesn't need a reference.
* 5. Set whichever client you want to run as startup and go!
*
* Required Files:
* ===============
* FileMgrCom_i.h FileMgrCom_i.c FileMgrCom.dll 
* TextSearchCom_i.h TextSearchCom_i.c TextSearchCom.dll
*
* Maintanence History:
* ====================
* ver 1.0 - 20 Mar 2016
* - first release
*/


#include <atlbase.h>
#include "../FileMgrCom/FileMgrCom_i.h"			// declarations generated from FileMgrCom.idl by MIDL
#include "../TextSearchCom/TextSearchCom_i.h"	// declarations generated from TextSearchCom.idl by MIDL
#include "../CommandlineParser/CommandLineParser.h"
/*
*  Note: - You have to include FileMgrCom_i.c and TextSearchCom_i.c in C++ client project.
*		   This file is generated MIDL from FileMgrCom.idl and TextSearchCom.idl and 
*		   contains definitions of the component's GUIDs.
*        - Don't include ATL header FileMgr.h - that's private to the component.
*/
#include <iostream>
#include <atlsafe.h>

#define FileSearchEnd L"FileSearchEnd"
#define TextSearchEnd L"TextSearchEnd"


void main(int argc, char *argv[])
{
	std::wcout << L"\n  Demonstrating Project #1 - Parallel Text Search Component with C++ Client";
	std::wcout << L"\n ==========================================================================\n";

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (!SUCCEEDED(hr))
	{
		std::wcout << L"\n  could not initialize COM";
	}

	try
	{
		// COM wrapper objects should be embedded inside a scope
		// between calls to CoInitialize(Ex) and CoUninitialize
		// so that their destructors get called BEFORE the COM
		// libraries are shut down with CoUninitialize.
		// Doing this inside a try-catch block is a good way to
		// do that, because the wrappers will throw exceptions if
		// calls fail.
		//
		CComQIPtr<IFileMgr> pFileMgr;					// IFileMgr and CLSID_FileMgr are defined in FileMgr_i.h
		pFileMgr.CoCreateInstance(CLSID_FileMgr);
		CComQIPtr<ITextSearch> pTextSearcher;				// ITextSearch and CLSID_TextSearch are defined in TextSearch_i.h
		pTextSearcher.CoCreateInstance(CLSID_TextSearch);

		if (pFileMgr != 0 && pTextSearcher != 0)
		{
			//------------<binding our sink to the component's connection point>------------
			HRESULT hr;
			IConnectionPointContainer* pCPCFileMgr;
			hr = pFileMgr->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPCFileMgr);
			HRESULT hr2;
			IConnectionPointContainer* pCPCTextSearcher;
			hr2 = pTextSearcher->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPCTextSearcher);

			//------------<Parse commandline arguments>------------

			CommandLineParser cmdLineParser;
			cmdLineParser.parseCommandLine(argc, argv);
			std::string path_ = cmdLineParser.getPath();
			std::string wantedtext_ = cmdLineParser.getwantedtext();
			std::vector<std::string> filePatterns = cmdLineParser.getPatterns();
			int threadNum = cmdLineParser.getThreadNum();


			//------------<set path and patterns to search files>------------
			for (std::string pattern_ : filePatterns)
			{
				CComBSTR pattern(pattern_.c_str());
				pFileMgr->addPattern(pattern);
			}
			CComBSTR path(path_.c_str());
			pFileMgr->searchFiles(path);

			pTextSearcher->SetThreadNum(3);
			//------------<print out patterns and path>------------
			std::wcout << L"\n  " <<threadNum << L" threads working in TextSearcher COM \n";
			std::wcout << L"\n  File patterns: ";
			for (std::string pattern_ : filePatterns)
			{
				std::cout << pattern_ + ", ";
			}
			std::wcout << L" search in Path:";
			std::cout << path_ + "\n";

			//------------<Search files in particular patterns and path>------------
			CComBSTR FileSearchEndSig(FileSearchEnd);
			while (true)
			{
				CComBSTR returnFile;
				pFileMgr->get(&returnFile);
				pTextSearcher->put(returnFile);
				if (returnFile == FileSearchEndSig) {
					break;
				}
				std::wcout << "\n  " << returnFile.Detach();
			}

			//------------<Search text from file search result>------------
			std::wcout << L"\n\n\n  Search text \"";
			std::cout << wantedtext_;
			std::wcout << L"\" results from above filelist: \n";

			CComBSTR wantedtext(wantedtext_.c_str());
			pTextSearcher->searchText(wantedtext);

			CComBSTR TextSearchEndSig(TextSearchEnd);
			while (true)
			{
				CComBSTR resultFile;
				pTextSearcher->get(&resultFile);
				if (resultFile == TextSearchEndSig) {
					break;
				}
				std::wcout << "\n  " << resultFile.Detach();
			}
		}
		else
		{
			throw std::exception("CoCreateInstance failed");
		}
		// Don't call release with pStrings.  CComQIPtr destructor takes
		// care of that.
	}
	catch (std::exception& ex)
	{
		std::wcout << L"\n  " << ex.what() << L"\n\n";
		return;
	}
	std::wcout << L"\n\n";
	CoUninitialize();
}