@echo off
set CC=g++
set SRC_COMMON=../_COMMON

%CC% nduy.cpp ^
     %SRC_COMMON%/Log.cpp ^
-o nduy.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe
