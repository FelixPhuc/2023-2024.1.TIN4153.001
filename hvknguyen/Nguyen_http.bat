@echo off
set CC=g++
set SRC_COMMON=../_COMMON

%CC% Nguyen_http.cpp ^
     %SRC_COMMON%/Log.cpp ^
     %SRC_COMMON%/Utils.cpp ^
-l ws2_32 ^
-o Nguyen_http.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe
REM g++ .\Nguyen_http.cpp ..\_COMMON\Log.cpp -o Nguyen_http.exe