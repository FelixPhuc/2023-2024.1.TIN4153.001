@echo off
set CC=g++
set SRC_COMMON=../_COMMON

%CC% giakhanh.cpp ^
     %SRC_COMMON%/Log.cpp ^
-o giakhanh.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe
REM g++ .\baitap1.cpp ..\_COMMON\Log.cpp -o baitap1.exe