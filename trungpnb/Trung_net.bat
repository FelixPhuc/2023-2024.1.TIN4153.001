@echo off
set CC=g++
set SRC_COMMON=p ^
     %SRC_COMMON%/Log.cpp ^
     %SRC_COMMON%/Utils.cpp ^
-l ws2_32 ^../_COMMON

%CC% Trung_http.cp
-o Trung_http.exe
REM g++ main.cpp ../../_src/Log.cpp -o t.exe
REM g++ .\Trung_http.cpp ..\_COMMON\Log.cpp -o Trung_http.exe