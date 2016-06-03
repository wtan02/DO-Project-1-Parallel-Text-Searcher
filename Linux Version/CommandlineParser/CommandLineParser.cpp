/////////////////////////////////////////////////////////////////////////////
// CommandLineParser.cpp -Parses Arguments From CommandLine                //
// ver 1.0                                                                 //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Thinkpad x230i, Core i3, Windows 8.1 pro                   //
// Application: CSE687 Project#1, File Catalogue                           //
// Author:      Wenchang Tan, MS of CE, Syracuse University                //
//              wtan02@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////

#include"CommandLineParser.h"
#include"../FileSystem/FileSystem.h"

#include<iostream> 

//--------------<  Parse CommandLine arguments into string vector >------------------
void CommandLineParser::parseCommandLine(const int& argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    if (args.size() > 1)
    {
        path = args[1];
        ParseCommandLineHelper(args);
    }
    else
        std::cout << "  Error, Please input CommandLine Arguments" << std::endl;

    return;
}

//--------------<  Parse vector into options  >-----------------
void CommandLineParser::ParseCommandLineHelper(std::vector<std::string> arguments)
{
    for (unsigned int i = 2; i < arguments.size(); i++)
    {
        //following with '/f' is an search option 
        if (arguments[i].substr(0, 2) == "/f" || arguments[i].substr(0, 2) == "/F")
        {
            options.push_back("/f");
            int keyposition = i + 1;
            wantedtext = arguments[i].substr(2, arguments[i].length() - 2);//remaining are keywords
        }
        else if (arguments[i].substr(0, 2) == "/t" || arguments[i].substr(0, 2) == "/T")
        {
            options.push_back("/t");
            int keyposition = i + 1;
            std::string::size_type sz;   // alias of size_t
            threadNum= std::stoi(arguments[i].substr(2, arguments[i].length() - 2), &sz);
        }
        else if (arguments[i].substr(0, 1) == "/")//following with '/' is a other option 
            options.push_back(arguments[i]);
        else if(arguments[i].substr(0, 2) == "*.")//following with "*." is a pattern
            filePatterns.push_back(arguments[i]);
    }
}

//--------------< checks whether given path is valid or not >-----------------
bool CommandLineParser::validatePath()
{
    using namespace FileSystem;
    if (!Directory::exists(path))
        return false;
    else
        return true;
}
int CommandLineParser::getThreadNum()
{
    return threadNum;
}
//--------------< checks whether a particular option is contained in command line argument or not >-----------------
bool CommandLineParser::isOptionPresent(std::string token)
{
    for (std::string option : options)
    {
        if (option == token)
            return true;
    }
    return false;
}

//--------------< Gets path after command line parsing >-----------------
std::string CommandLineParser::getPath()
{
    return path;
}

//--------------< Gets the patterns collection after command line parsing >-----------------
std::vector<std::string> CommandLineParser::getPatterns()
{
    return filePatterns;
}

//--------------< Gets the keywords after command line parsing >-----------------
std::string CommandLineParser::getwantedtext()
{
    return wantedtext;
}

//--------------< Gets the options collection after command line parsing >-----------------
std::vector<std::string> CommandLineParser::getOptions()
{
    return options;
}

#ifdef TEST_COMMANDLINPARSER
void main(int argc, char *argv[])
//{
//	CommandLineParser cmdLineParser;
//	std::vector <std::string>arguments;
//	for (int i = 0; i < argc; i++)
//		arguments.push_back(argv[i]);
//
//	cmdLineParser.parseCommandLine(argc, argv);
//	std::string path = cmdLineParser.getPath();
//	std::string wantedtext = cmdLineParser.getwantedtext();
//	std::vector<std::string> options = cmdLineParser.getOptions();
//	std::vector<std::string> filePatterns = cmdLineParser.getPatterns();
//
//	std::cout << "\n  Parsing Command Line Arguments:";
//	std::cout << "\n --------------------------------";
//	std::cout << "\n  Path:\t " + path;
//
//	std::cout << "\n  Options:  ";
//	for (std::string option : options)
//		std::cout << option + "  ";
//	std::cout << std::endl;
//	std::cout << "  Keywords:\t " + wantedtext;
//	//validating command line arguments
//	if (cmdLineParser.validatePath())
//	std::cout<<"\n\n  Command Line Arguments contains 'valid existing Path.";
//	else
//	std::cout<<"\n\n  Command Line Arguments does not contain 'valid existing Path'.";
//	std::cout << std::endl;
//	getchar();
//}
#endif