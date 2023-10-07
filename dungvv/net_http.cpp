#include <stdio.h>
#include "..\_COMMON\Log.h"
#include "../_COMMON/Utils.h"
#include "../_COMMON/uri.h"

#include "winsock2.h"
#include "ws2tcpip.h"
#include "windows.h"
 
//Hàm khởi tạo WinSock
bool InitWinSock(){
  WSADATA wsaData;
  WORD wVersion = MAKEWORD(2,2);
  int err = WSAStartup(wVersion,&wsaData);
  if (err != 0) {
    LOG_ET("WSAStartup() error: %d\n", err);
    return false;
  }
  LOG_DT("[-] Winsock khoi tao thanh cong\n");
  return true;
}

//hàm lấy địa chỉ IPv4 của domain tương ứng
//muc đích: IPv4 nhận được sử dụng cho hàm connect()
std::string getIpAddress(const std::string& domain){
  LOG_DT("[CALL] getIpAddress(%s)\n",domain.c_str());
  struct hostent *remoteHost;
  remoteHost = gethostbyname(domain.c_str());
  if (remoteHost == NULL) {
    LOG_ET("gethostbyname() error: %ld\n", WSAGetLastError());
    return std::string();
  }
  struct in_addr addr;
  addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];

  std::string ip(inet_ntoa(addr));
  LOG_WT("IP OF DOMAIN [%s] => [%s]\n",domain.c_str(),ip.c_str());
  return ip;
}
//Hiển thị các thành phần của URI 
void showUri(const std::string& url){
  Uri u = Uri::Parse(url);
  LOG_I("URL [%s]\n",url.c_str());
  LOG_W("protocol[%s]\nhost[%s]\nport[%d]\npath[%s]\nquery[%s]\n",
    u.Protocol.c_str(),
    u.Host.c_str(),
    u.getPort(),
    u.getPath().c_str(),
    u.QueryString.c_str());

    getIpAddress(u.Host);
}
//kiểm thử các URI
void test_uri(){
  showUri("https://stackoverflow.com/questions/2616011/easy-way-to-parse-a-url-in-c-cross-platform");
  showUri("http://localhost:80/foo.html?&q=1:2:3");
  showUri("https://localhost:80/foo.html?&q=1");
  showUri("localhost/foo");
  showUri("https://localhost/foo");
  showUri("localhost:8080");
  showUri("localhost?&foo=1");
  showUri("localhost?&foo=1:2:3"); 
  showUri("https://github.com/vvdung-husc/2022-2023.1.TIN4153.001");
  showUri("http://tuyensinh.husc.edu.vn/");  

  LOG_D("");
}
int main(int argc, char const *argv[])
{
  LOG_WT("*** NETWORK HTTP CLIENT ***\n");

  //test_uri();

  LOG_IT("[*] Khoi tao WinSocket\n");
  if (!InitWinSock()) return -1;

  //showUri("http://danang.edu.vn/documents/11834/0/NAMHOC2023-2024.png/d54daaad-848a-4db4-9c71-13a18d245d77?t=1693884664521");
  test_uri();
  return -1;

  std::string domain = "oj.husc.edu.vn";
  std::string ip = getIpAddress(domain);

  if (ip.empty()){
    LOG_ET("Ten mien [%s] khong ton tai. Error:%d\n",domain.c_str(),WSAGetLastError());
    LOG_D("");
    return -1;
  }
  
  LOG_WT("IPv4 cua ten mien [%s] => [%s] \n",domain.c_str(),ip.c_str());


  //Clean Winsock trước khi thoát ứng dụng
  WSACleanup();
  LOG_D("");
  return 0;
}
