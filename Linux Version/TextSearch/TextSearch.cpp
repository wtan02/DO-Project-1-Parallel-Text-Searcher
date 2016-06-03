
#include "TextSearch.h"
#include "ITextSearch.h"
#include "../FileSystem/FileSystem.h"
#include <thread>
#define FileSearchEnd "FileSearchEnd"
#define TextSearchEnd "TextSearchEnd"
#define Active true;
#define Sleep false;
// CTextSearch
bool TextSearch::Allocated = false;
bool TextSearch::searching = false;
//-----------<Constructor>--------------
TextSearch::TextSearch()
{
    exitSearch = false;
}
//-----------<Destructor>--------------
TextSearch::~TextSearch()
{
    for (auto& thread : threadPool)
    {
        thread.second = Active;
    }
    exitSearch = true;
    cv_.notify_all();
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
}
//-----------<Input files to inputQueue>--------------
void TextSearch::put(std::string filePath)
{
    if (filePath == FileSearchEnd)
    {
        for (size_t threadNum = 0; threadNum < threadPool.size(); threadNum++) {
            inputQueue.enQ(FileSearchEnd);
        }
    }
    else 
    {
        inputQueue.enQ(filePath);
    }
    return;
}

//-----------<Get a result from resultQueue>--------------
std::string TextSearch::get()
{
    std::string resultFilePath = resultQueue.deQ();;
    return resultFilePath;
}

//-----------<Get number of working thread>--------------
void TextSearch::SetThreadNum(int Num)
{
	// TODO: Add your implementation code here
    
    if (!Allocated)
    {
        Allocated = true;
        for (long i = 0; i < Num; i++)
        {
            
            std::lock_guard<std::mutex> l(ioLock);

            std::thread childThread([&]()
            {
                doSearch();
            });
            
            std::cout << "\n  Text search thread " << childThread.get_id() <<" was created"<< std::endl;
            if (threadPool.find(childThread.get_id()) == threadPool.end()){
                threadPool[childThread.get_id()] = Active;
            }
            
            childThread.detach();
			inputQueue.enQ(FileSearchEnd);
        }
    }
    else 
    {
        std::lock_guard<std::mutex> l(ioLock);
        std::cout << "\n  Can't set number of threads again!" << std::endl;
    }
    return;
}
//-----------<thread proxcy>--------------
void TextSearch::doSearch()
{
    while (!exitSearch)
    {
        std::string file = inputQueue.deQ();
        if (file == FileSearchEnd)
        {
	    std::this_thread::sleep_for(std::chrono::milliseconds(3));
            threadPool[std::this_thread::get_id()] = Sleep;
            std::unique_lock<std::mutex> l(mtx_);
            while (!threadPool[std::this_thread::get_id()]) 
                cv_.wait(l, [this]() { return threadPool[std::this_thread::get_id()]; });
        }
        else
        {
            if (searchInFile(file))
                resultQueue.enQ(file);
        }
    }
}
//-----------<Search particular text in files>--------------
bool TextSearch::searchInFile(std::string filename)
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
void TextSearch::searchText(std::string text)
{
    if (!searching)
    {
        searching = true;
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

    return;
}
//-----------<Check threading status>--------------
void TextSearch::checkSearchStatus()
{
    while (inputQueue.size() > 0) 
    {
        ;
    }
    resultQueue.enQ(TextSearchEnd);
}
//-----------<Create a instance of TextSearch object>--------------
ITextSearch* ITextSearch::createInstance()
{
    return new TextSearch;
}
