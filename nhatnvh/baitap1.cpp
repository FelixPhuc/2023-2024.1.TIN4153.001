#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. nhatnvh\n");
  LOG_I("WELCOME C++. nhatnvh\n");
  LOG_W("WELCOME C++. nhatnvh\n");
  LOG_E("WELCOME C++. nhatnvh\n");

  LOG_DT("WELCOME C++. nhatnvh\n");
  LOG_IT("WELCOME C++. nhatnvh\n");
  LOG_WT("WELCOME C++. nhatnvh\n");
  LOG_ET("WELCOME C++. nhatnvh\n");

  LOG_D("");
  return 0;
}