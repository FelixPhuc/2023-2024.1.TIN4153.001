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
#include <cstdint>
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

int main(int argc, char const *argv[])
{
  LOG_WT("*** SERVER 1 -- Tran Van Thong - 20T1020574 ***\n");
  LOG_WT("* Su dung TELNET de ket noi den SERVER\n");
  LOG_WT("* telnet IP_SERVER PORT_NUMBER\n");
  LOG_WT("* Nhan Ctrl+] de su dung options\n");

  LOG_IT("[*] Khoi tao WinSocket\n");
  if (!InitWinSock()) return -1;
  
  //Các biến dùng cho gửi/nhận dữ liệu
  int iSendResult;
  char recvbuf[MAX_BUFFER_SIZE];
  int recvbuflen = MAX_BUFFER_SIZE;

  //Chuẩn bị thông tin server
  //Tạo socket cho server để lắng nghe chờ các client kết nối
  SOCKET srvSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); 
  if (srvSock == INVALID_SOCKET) {
    LOG_ET("socket failed with error: %ld\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }  
  //Cài đặt dịch vụ lắng nghe tại cổng PORT_LISTEN
  struct sockaddr_in srvAddress;
  ZeroMemory(&srvAddress, sizeof(srvAddress));
  srvAddress.sin_family = AF_INET;            //IPv4
  srvAddress.sin_addr.s_addr = INADDR_ANY;    //địa chỉ bất kỳ 0.0.0.0
  srvAddress.sin_port  = htons(PORT_LISTEN);    //số hiệu cổng lắng nghe

  //Ràng buộc cổng dịch vụ cho hệ thống nhận biết
  int iResult = bind( srvSock, (struct sockaddr*)&srvAddress, sizeof(srvAddress));
  if (iResult == SOCKET_ERROR) {
    LOG_ET("bind failed with error: %d\n", WSAGetLastError());    
    closesocket(srvSock);
    WSACleanup();
    return 1;
  }
  
  //Lắng nghe chờ kết nối từ client
  if (listen(srvSock, SOMAXCONN) < 0) {
    closesocket(srvSock);
    WSACleanup();
    return 1;
  }

  //Chờ client kết nối
  LOG_WT("Server dang lang nghe tai cong so: %d\n",PORT_LISTEN);

  struct sockaddr_in cltAddress;
  int addrlen = (int)sizeof(cltAddress);
  SOCKET cltSock = accept(srvSock, (struct sockaddr*)&cltAddress,&addrlen);
  if (cltSock == INVALID_SOCKET) {
    LOG_ET("accept failed with error: %d\n", WSAGetLastError());
    closesocket(srvSock);
    WSACleanup();
    return 1;
  }
  //Không dùng đến server -> đóng cổng (trường hợp chỉ kết nối 1 client)
  //closesocket(srvSock);  

  uint8_t* pIP = (uint8_t*)&cltAddress.sin_addr.s_addr;
  std::string szIP; 
  int cltPort = ntohs(cltAddress.sin_port) ;
  szIP = StringFormat("%d.%d.%d.%d",pIP[0],pIP[1],pIP[2],pIP[3]);
  LOG_W("Client: %s:%d connected\n",szIP.c_str(),cltPort );

  // Receive until the peer shuts down the connection
  do {

    iResult = recv(cltSock, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
      recvbuf[iResult] = 0;
      LOG_D("So byte nhan duoc: %d - noi dung\n%s\n", iResult,recvbuf);  
      // Kiểm tra xem chuỗi nhận được có phải là "exit" không
    if (strcmp(recvbuf, "exit") == 0) {
      LOG_IT("Client requested to exit. Closing connection [%s:%d]...\n", szIP.c_str(), cltPort);
      break; // Thoát vòng lặp và đóng kết nối
    }   
    }
    else if (iResult == 0)
      LOG_IT("Connection closing [%s:%d]...\n",szIP.c_str(),cltPort);
    else  {
      LOG_ET("recv failed with error: %d\n", WSAGetLastError());
      closesocket(cltSock);
      WSACleanup();
      return 1;
    }

  } while (iResult > 0);


  closesocket(cltSock);
  //Clean Winsock trước khi thoát ứng dụng
  WSACleanup();
  LOG_IT("[*] --- Thoat ung dung --- [*]\n");
  LOG_D("");
  return 0;
}