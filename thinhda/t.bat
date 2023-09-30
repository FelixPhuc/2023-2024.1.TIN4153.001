@echo off
set CC=g++
set SRC_COMMON=../_COMMON

%CC% thinhda.cpp ^
     %SRC_COMMON%/Log.cpp ^
-o thinhda.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe
REM g++ thinhda.cpp ../_COMMON/Log.cpp -o thinhda.exe