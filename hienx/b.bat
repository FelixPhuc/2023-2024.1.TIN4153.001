@echo off
set CC=g++
set SRC_COMMON=../_COMMON

%CC% hienx.cpp ^
     %SRC_COMMON%/Log.cpp ^

-o hienx.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe