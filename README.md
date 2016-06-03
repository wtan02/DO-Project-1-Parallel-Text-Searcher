Project #1 - Parallel Text Search Component
Version 1.0, Revised: 01/25/2016 17:32:17
Due Date: Tuesday, March 22nd

Purpose:

Components are usually relatively small pieces of software built as libraries that expose an interface for accessing functionality and an object factory that allows a client to initialize the software by creating internal objects without binding to concrete definitions. Often a component provides to a client the means to manage the lifetime of internal objects and to specify that the component is no longer needed so it's binary library may be unloaded by the process.

Microsoft developed, in the 90's, a Windows platform technology it calls the Component Object Model (COM) that is based on components. Its use is pervasive throughout their product ecosystem, e.g., Windows, the .Net framework, Office, IIS and many of its other enterprise server technologies.

In this project we will develop a Parallel Text Search program using components on both Windows and the Linux operating system. For the Windows-based program we will use the Microsoft COM technology. Linux does not support COM so we will build our own component infrastructure for that environment.

For both programs we will use C++11 threading to support searching for text in multiple files concurrently.

Requirements:

Your Parallel Text Search Programs:

1.Shall be implemented in C++11 using Visual Studio 2015 for the COM-on-Windows program and g++ ver 5.3 for the Components-on-Linux program1.

2.Shall, on each platform, provide a console application that, given a search string, a path, and set of file name patterns, identifies by name and path all those files matching one or more of the patterns and which contain an instance of the search string.

3.Shall provide a FileMgr component that finds the names of all the files residing in a directory tree rooted at the specified path and matching one or more of the patterns. It exposes these results by providing a get() method that retrieves each result from an internal thread-safe blocking queue.

4.Shall provide a text search component that enqueues fully qualified file name inputs and creates a specified number of threads used to dequeue filenames and search for a specified string asynchronously. If a file contains the specified string its fully qualified filename is enqueued for output by the owning thread, which then attempts to dequeue another filename.

5.Shall use a Powershell script on windows and a Bash script on Linux to demonstrate that the components implement their required functionality. The combination of scripts and console applications should clearly show the initial requests, the output of the FileMgr, and the output of the file search processing.

6.Please use either NetBeans or Eclipse IDEs to develop your project code for the Linux platform. You may use CLang instead of g++ if you wish, provided that you have a version that implements C++11 threading features.

7.What you need to know:

In order to successfully meet these requirements you will:

1.Install Linux, presumably in a Virtual Machine. I use Ubuntu in a VMWare Player virtual machine. You may use any distribution you like and might choose to use a VirtualBox virtual machine instead of VMWare Player.

2.Know or learn how C++ classes and class hierarchies work, C++11 threading, and how to design interfaces and object factories. We will discuss examples of these in class.

3.How COM works and how to use the ATL libraries. We will discuss this in detail in the first few weeks of classes.
