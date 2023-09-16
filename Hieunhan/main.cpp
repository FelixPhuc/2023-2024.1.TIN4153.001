#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. Hieunhan\n");
  LOG_I("WELCOME C++. Hieunhan\n");
  LOG_W("WELCOME C++. Hieunhan\n");
  LOG_E("WELCOME C++. Hieunhan\n");

  LOG_DT("WELCOME C++. Hieunhan\n");
  LOG_IT("WELCOME C++. Hieunhan\n");
  LOG_WT("WELCOME C++. Hieunhan\n");
  LOG_ET("WELCOME C++. Hieunhan\n");

  LOG_D("");
  return 0;
}
