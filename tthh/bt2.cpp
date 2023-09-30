#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. thanhhh\n");
  LOG_I("WELCOME C++. thanhhh\n");
  LOG_W("WELCOME C++. thanhhh\n");
  LOG_E("WELCOME C++. thanhhh\n");

  LOG_DT("WELCOME C++. thanhhh\n");
  LOG_IT("WELCOME C++. thanhhh\n");
  LOG_WT("WELCOME C++. thanhhh\n");
  LOG_ET("WELCOME C++. thanhhh\n");

  LOG_D("");
  return 0;
}