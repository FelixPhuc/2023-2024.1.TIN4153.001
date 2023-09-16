#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. khanh\n");
  LOG_I("WELCOME C++. khanh\n");
  LOG_W("WELCOME C++. khanh\n");
  LOG_E("WELCOME C++. khanh\n");

  LOG_DT("WELCOME C++. khanh\n");
  LOG_IT("WELCOME C++. khanh\n");
  LOG_WT("WELCOME C++. khanh\n");
  LOG_ET("WELCOME C++. khanh\n");

  LOG_D("");
  return 0;
}