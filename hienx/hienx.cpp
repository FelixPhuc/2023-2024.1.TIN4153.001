#include <stdio.h>
#include "..\_COMMON\Log.h"

int main(int argc, char const *argv[])
{
  printf("HELLO C++\n");
  LOG_D("WELCOME C++. hienxx\n");
  LOG_I("WELCOME C++. hienxx\n");
  LOG_W("WELCOME C++. hienxx\n");
  LOG_E("WELCOME C++. hienxx\n");

  LOG_DT("WELCOME C++. hienxx\n");
  LOG_IT("WELCOME C++. hienxx\n");
  LOG_WT("WELCOME C++. hienxx\n");
  LOG_ET("WELCOME C++. hienxx\n");

  LOG_D("");
  return 0;
}
