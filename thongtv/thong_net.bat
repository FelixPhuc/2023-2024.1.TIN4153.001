@echo off
set CC=g++
set SRC_COMMON=../_COMMON

%CC% thong_http.cpp ^
     %SRC_COMMON%/Log.cpp ^
     %SRC_COMMON%/Utils.cpp ^
-l ws2_32 ^
-o thong_http.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe
REM g++ .\baitap1.cpp ..\_COMMON\Log.cpp -o baitap1.exe