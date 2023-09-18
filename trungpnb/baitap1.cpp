#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. Trungpnb\n");
  LOG_I("WELCOME C++. Trungpnb\n");
  LOG_W("WELCOME C++. Trungpnb\n");
  LOG_E("WELCOME C++. Trungpnb\n");

  LOG_DT("WELCOME C++. Trungpnb\n");
  LOG_IT("WELCOME C++. Trungpnb\n");
  LOG_WT("WELCOME C++. Trungpnb\n");
  LOG_ET("WELCOME C++. Trungpnb\n");

  LOG_D("");
  return 0;
}