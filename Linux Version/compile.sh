cd ./FileSystem
g++ -std=c++0x -pthread -fPIC -c -g FileSystem.cpp -o FileSystem.o
cd ../BlockingQueue
g++ -std=c++0x -pthread -fPIC -c -g Cpp11-BlockingQueue.cpp -o Cpp11-BlockingQueue.o
cd ../CommandlineParser
g++ -std=c++0x -pthread -fPIC -c -g CommandLineParser.cpp -o CommandLineParser.o
cd ../TextSearch
g++ -std=c++0x -pthread -fPIC -c -g TextSearch.cpp -o TextSearch.o
cd ../FileMgr
g++ -std=c++0x -pthread -fPIC -c -g FileMgr.cpp -o FileMgr.o
cd ../
g++ -std=c++0x -pthread -c -g ./Parallel\ Text\ Searcher/main.cpp  -o main.o

g++ -std=c++0x -pthread -o libTextSearch.so ./FileSystem/FileSystem.o ./FileMgr/FileMgr.o ./BlockingQueue/Cpp11-BlockingQueue.o -shared -fPIC
g++ -std=c++0x -pthread -o libFileMgr.so ./FileSystem/FileSystem.o ./FileMgr/FileMgr.o ./BlockingQueue/Cpp11-BlockingQueue.o -shared -fPIC


g++ -std=c++0x -pthread main.o ./CommandlineParser/CommandLineParser.o -o main -Wl,-rpath,./ -L./ -lTextSearch -lFileMgr 
chmod 777 main

