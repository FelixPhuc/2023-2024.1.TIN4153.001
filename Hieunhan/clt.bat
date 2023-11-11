@echo off
set CC=g++
set SRC_COMMON=../_COMMON
set BINNAME=client1
REM Quy định BINNAME chính là tên file có chứa hàm main()

%CC% %BINNAME%.cpp ^
     %SRC_COMMON%/Log.cpp ^
     %SRC_COMMON%/Utils.cpp ^
-l ws2_32 ^
-o %BINNAME%.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe
REM g++ .\main.cpp ..\_COMMON\Log.cpp -o h.exe