#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. THONGTV\n");
  LOG_I("WELCOME C++. THONGTV\n");
  LOG_W("WELCOME C++. THONGTV\n");
  LOG_E("WELCOME C++. THONGTV\n");

  LOG_DT("WELCOME C++. THONGTV\n");
  LOG_IT("WELCOME C++. THONGTV\n");
  LOG_WT("WELCOME C++. THONGTV\n");
  LOG_ET("WELCOME C++. THONGTV\n");

  LOG_D("");
  return 0;
}