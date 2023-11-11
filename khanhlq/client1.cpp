#include <stdio.h>
#include "..\_COMMON\Log.h"
#include "../_COMMON/Utils.h"
#include "../_COMMON/uri.h"

#include "winsock2.h"
#include "ws2tcpip.h"
#include "windows.h"

#include <fstream>
#include <regex>
#include <math.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE   2048
#define PORT_LISTEN       12345

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

void Usage(const char* binName){
  LOG_WT("*** USAGE - LE QUANG KHANH***\n");
  LOG_IT("* %s <ipserver> <port>\n",binName);
  LOG_WT("* Enter 'exit' or Press 'Ctrl + C' to exit application\n");
}

void ShowArgs(int argc, char const *argv[]){  
  LOG_WT("COUNT Arguments:%d\n",argc);
  for (int i =0; i < argc; ++i){
    LOG_IT(" %2d - %s\n",i + 1,argv[i]);
  }  
  LOG_D("\n");
}

int main(int argc, char const *argv[])
{
  
  ShowArgs(argc,argv);

  if (argc < 3){

    Usage(argv[0]); return 0;
  }

  LOG_IT("[*] Khoi tao WinSocket\n");
  if (!InitWinSock()) return -1;
  
  std::string ipserver = argv[1];
  int portServer = atoi(argv[2]);

  //Các biến dùng cho gửi/nhận dữ liệu
  int iSendResult;
  char recvbuf[MAX_BUFFER_SIZE];
  int recvbuflen = MAX_BUFFER_SIZE;

  //Chuẩn bị thông tin server
  //Tạo socket cho server để lắng nghe chờ các client kết nối
  SOCKET cltSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); 
  if (cltSock == INVALID_SOCKET) {
    LOG_ET("socket failed with error: %ld\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }  
  //Cài đặt thông tin server
  struct sockaddr_in srvAddress;
  ZeroMemory(&srvAddress, sizeof(srvAddress));
  srvAddress.sin_family = AF_INET;            //IPv4
  inet_pton(AF_INET,ipserver.c_str(),&srvAddress.sin_addr.s_addr);  
  srvAddress.sin_port  = htons(portServer);    //số hiệu cổng lắng nghe

  //Kết nối đến server đã cấu hình 
  int iResult = connect(cltSock, (SOCKADDR *) & srvAddress, sizeof (srvAddress));
  if (iResult == SOCKET_ERROR) {
    LOG_ET("connect failed with error: %d\n", WSAGetLastError());    
    closesocket(cltSock);
    WSACleanup();
    LOG_D("\n");
    return 1;
  }
  
  LOG_WT("Connected to server.\n");
  char msg[1024];
  do {
    LOG_DT("Nhap noi dung:");
    gets(msg);

    iResult = send(cltSock, msg, sizeof(msg), 0);
    if (iResult <= 0)
    {
      if (iResult == 0)  LOG_IT("Connection closing ...\n");
      else {
        LOG_ET("send failed with error: %d\n", WSAGetLastError());
      }
      break;    
    }

    if (strcmp(msg,"exit") == 0) break;
  
  } while (true);

  closesocket(cltSock);
  //Clean Winsock trước khi thoát ứng dụng
  WSACleanup();
  LOG_IT("[*] --- Thoat ung dung --- [*]\n");
  LOG_D("");
  return 0;
}