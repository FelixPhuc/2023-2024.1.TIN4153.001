#include <stdio.h>
#include "..\_COMMON\Log.h"
#include "../_COMMON/Utils.h"

#include "winsock2.h"
#include "ws2tcpip.h"
#include "windows.h"

//hàm lấy địa chỉ IPv4 của domain tương ứng
//muc đích: IPv4 nhận được sử dụng cho hàm connect()
std::string getIpAddress(const std::string& domain){
  LOG_DT("[CALL] getIpAddress(%s)\n",domain.c_str());
  struct hostent *remoteHost;
  remoteHost = gethostbyname(domain.c_str());
  if (remoteHost == NULL) {
    //LOG_ET("gethostbyname() error: %ld\n", WSAGetLastError());
    return std::string();
  }
  struct in_addr addr;
  addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];

  std::string ip(inet_ntoa(addr));
  //LOG_WT("IP OF DOMAIN [%s] => [%s]\n",domain.c_str(),ip.c_str());
  return ip;
}

int main(int argc, char const *argv[])
{
  LOG_D("WELCOME C++ - NETWORK HTTP CLIENT\n");

  LOG_IT("1. Khoi tao WinSocket\n");
  WSADATA wsaData;
  WORD wVersion = MAKEWORD(2,2);
  int err = WSAStartup(wVersion,&wsaData);
  if (err != 0) {
    LOG_ET("WSAStartup() loi: %d\n", err);
    return 1;
  }
  LOG_DT("[-] Winsock khoi tao thanh cong\n");

  std::string domain = "oj.husc.edu.vn";
  std::string ip = getIpAddress(domain);

  if (ip.empty()){
    LOG_ET("Ten mien [%s] khong ton tai. Error:%d\n",domain.c_str(),WSAGetLastError());
    LOG_D("");
    return -1;
  }
    
  else 
    LOG_WT("IPv4 cua ten mien [%s] => [%s] \n",domain.c_str(),ip.c_str());

  
  WSACleanup();
  LOG_D("");
  return 0;
}
