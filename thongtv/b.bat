@echo off
set CC=g++
set SRC_COMMON=../_COMMON

%CC% baitap1.cpp ^
     %SRC_COMMON%/Log.cpp ^
-o baitap1.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe
REM g++ .\baitap1.cpp ..\_COMMON\Log.cpp -o baitap1.exe