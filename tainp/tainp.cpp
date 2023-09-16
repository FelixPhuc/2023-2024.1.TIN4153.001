#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. NpTai\n");
  LOG_I("WELCOME C++. NpTai\n");
  LOG_W("WELCOME C++. NpTai\n");
  LOG_E("WELCOME C++. NpTai\n");

  LOG_DT("WELCOME C++. NpTai\n");
  LOG_IT("WELCOME C++. NpTai\n");
  LOG_WT("WELCOME C++. NpTai\n");
  LOG_ET("WELCOME C++. NpTai\n");

  LOG_D("");
  return 0;
}
