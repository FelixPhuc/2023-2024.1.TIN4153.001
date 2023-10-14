@echo off
set CC=g++
set SRC_COMMON=../_COMMON

%CC% baitap1.cpp ^
     %SRC_COMMON%/Log.cpp ^
     %SRC_COMMON%/Utils.cpp ^
-l ws2_32 ^
-o bt1.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe
REM g++ .\baitap1.cpp ..\_COMMON\Log.cpp -o baitap1.exe