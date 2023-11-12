#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. yeu anh duong\n");
  LOG_I("WELCOME C++. THANH\n");
  LOG_W("WELCOME C++. THANH\n");
  LOG_E("WELCOME C++. THANH\n");

  LOG_DT("WELCOME C++. THANH\n");
  LOG_IT("WELCOME C++. THANH\n");
  LOG_WT("WELCOME C++. THANH\n");
  LOG_ET("WELCOME C++. THANH\n");

  LOG_D("");
  return 0;
}
