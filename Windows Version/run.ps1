cls

cd Debug
@Echo off
Echo Demo 3 threads working in TextSearch COM to search "public" in .cpp and .h files
"CppClient.exe" ../ public *.cpp *.h /f"public" /t"3"

Echo Demo 4 threads working in TextSearch COM to search "public" in .cpp and .h files
"CppClient.exe" ../ public *.cpp *.h /f"public" /t"3"
cd ../