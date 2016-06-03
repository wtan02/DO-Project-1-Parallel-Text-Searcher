/////////////////////////////////////////////////////////////////////////////
// CommandLineParser.h -Parses Arguments From CommandLine                  //
// ver 1.0                                                                 //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Thinkpad x230i, Core i3, Windows 8.1 pro                   //
// Application: CSE687 Project#1, File Catalogue                           //
// Author:      Wenchang Tan, MS of CE, Syracuse University                //
//              wtan02@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
/*
*Package Operations:
*===================
*Provides support for parsing command line into path, patterns and options.
*Defines CommandLineParser Class.
*
*Public Interfaces:
*==================
*parseCommandLine();	    //parses command line to single arguments.
*ParseCommandLineArgument();//parses every arguments to find path, patterns, options.
*validatePath();			//Validating path
*isOptionPresent();			//To check whether a particular option is passed in command line argument or not
*getPath();					//Gets the path argument from command line parsing
*getOptions();				//Gets the options collection from command line parsing
*getPatterns();				//Gets the patterns collection from command line parsing
*
*Required Files:
*===============
*CommandLineParser.h CommandLineParser.cpp
*
*Build Command:
*==============
*cl /Project1 /DTEST_COMMANDLINEPARSER CommandLineParser.cpp
*
*Maintanence History:
*====================
*ver 2.0 - 20 Mar 2016
*- add parsing thread number function
*ver 1.0 - 01 Feb 2015
*- first release
*/
#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H
 // !COMMANDLINEPARSER_H



#include<iostream>
#include <string>
#include <vector>


class CommandLineParser
{
public:
    void parseCommandLine(const int& argc, char *argv[]);
    void ParseCommandLineHelper(std::vector<std::string>);
    bool validatePath();
    bool isOptionPresent(std::string);
    std::string getPath();
    std::string getwantedtext();
    std::vector<std::string> getPatterns();
    std::vector<std::string> getOptions();
    int getThreadNum();

private:
    std::vector<std::string>args;
    std::string path;
    std::string wantedtext;
    int threadNum;
    std::vector<std::string> options;
    std::vector<std::string> filePatterns;
};
#endif
