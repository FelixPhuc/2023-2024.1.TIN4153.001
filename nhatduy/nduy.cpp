#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. NDUY\n");
  LOG_I("WELCOME C++. NDUY\n");
  LOG_W("WELCOME C++. NDUY\n");
  LOG_E("WELCOME C++. NDUY\n");

  LOG_DT("WELCOME C++. NDUY\n");
  LOG_IT("WELCOME C++. NDUY\n");
  LOG_WT("WELCOME C++. NDUY\n");
  LOG_ET("WELCOME C++. NDUY\n");

  LOG_D("");
  return 0;
}
