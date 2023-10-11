#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. nhatthanh\n");
  LOG_I("WELCOME C++. nhatthanh\n");
  LOG_W("WELCOME C++. nhatthanh\n");
  LOG_E("WELCOME C++. nhatthanh\n");

  LOG_DT("WELCOME C++. nhatthanh\n");
  LOG_IT("WELCOME C++. nhatthanh\n");
  LOG_WT("WELCOME C++. nhatthanh\n");
  LOG_ET("WELCOME C++. nhatthanh\n");

  LOG_D("");
  return 0;
}