@echo off
set CC=g++
set SRC_COMMON=../_COMMON

%CC% main.cpp ^
     %SRC_COMMON%/Log.cpp ^
-o h.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe
REM g++ .\main.cpp ..\_COMMON\Log.cpp -o h.exe