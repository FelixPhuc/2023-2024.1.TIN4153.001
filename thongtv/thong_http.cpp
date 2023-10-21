#include <stdio.h>
#include "..\_COMMON\Log.h"
#include "../_COMMON/Utils.h"
#include "../_COMMON/uri.h"

#include "winsock2.h"
#include "ws2tcpip.h"
#include "windows.h"

#include <fstream>

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

//Hàm lấy nội dung của URI host:port path
//tham số header lưu nội dung Header HTTP
std::string getContentSite(const std::string& host,int port,std::string path, std::string* header){
  //1. nhận IP từ host
  std::string ip = getIpAddress(host);
  //2. tạo socket chuẩn bị kết nối
  SOCKET sockConnect;
  sockConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockConnect == INVALID_SOCKET) {
    LOG_ET("socket() error: %d\n", WSAGetLastError());
    return std::string();
  }

  //3. Thông tin Server để kết nối gồm IP và PORT
  sockaddr_in srvService;
  srvService.sin_family = AF_INET;
  //srvService.sin_addr.s_addr = inet_addr(ip.c_str());
  inet_pton(AF_INET,ip.c_str(),&srvService.sin_addr.s_addr);
  srvService.sin_port = htons(port);
  
  //4. Kết nối đến server với thông tin ở trên
  int iResult = connect(sockConnect, (struct sockaddr *) &srvService, sizeof (srvService));
  if (iResult == SOCKET_ERROR) {
    LOG_ET("connect() error: %d\n", WSAGetLastError());
    closesocket(sockConnect);
    return std::string();;
  }

  //5. Chuẩn bị Request Header HTTP với tài nguyên đã cho PATH của URI
  if (path.empty()) path = "/";
  //std::string request = "GET / HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
  std::string request = StringFormat("GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",path.c_str(),host.c_str());
  if(send(sockConnect, request.c_str(), strlen(request.c_str())+1, 0) < 0){
    //LOG_ET("send() failed: %ld\n",WSAGetLastError());
  }
  
  //6. Nhận nội dung trả về từ Server
  std::string txt;
  char buffer[4097];
  int n, total = 0;
  std::string raw_site;
  while((n = recv(sockConnect, buffer, 4096, 0)) > 0){
    total += n;
    buffer[n] = 0;
    txt.append(buffer);
    //LOG_D("%s",buffer);
  }

  closesocket(sockConnect);

  //7. Tách Response Header HTTP và nội dung
  size_t pos = txt.find("\r\n\r\n");//4 kytu  
  //LOG_ET("POS:%zu\n",pos);
  if (header){
    *header = txt.substr(0,pos);
  }  
  std::string content = txt.substr(pos + 4);//4ky tu "\r\n\r\n"
  size_t t1 = content.find("<!DOCTYPE html>");
  if (t1 != std::string::npos){
    size_t t2 = content.rfind("</html>");
    if (t2 == std::string::npos) return std::string(content.substr(t1));
    return std::string(content.substr(t1,t2 - t1 + 7));
  }
  return std::string(content);//4ky tu "\r\n\r\n"
}

//Hàm lưu nội dung URI vào file
void saveHost2Html(const std::string& host,int port,std::string path, const std::string& file){  
  std::string header;
  std::string content = getContentSite(host,port,path,&header);
  LOG_D("[RESPONSE HEADER ]==========>\n%s\n",header.c_str());
  LOG_W("CONTENT SIZE:%zu SAVE[%s]\n",content.size(),file.c_str());
  std::ofstream fs;
  fs.open(file.c_str(),std::ofstream::out | std::ofstream::trunc);
  if (!fs.is_open()) {
    LOG_ET("fs.open() Error\n");
    return;
  }
  fs.write(content.c_str(),content.size());
  fs.close();
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
//kiểm thử hàm getIpAddress
void test_getIpAddress(){
  std::string domain = "oj.husc.edu.vn";
  std::string ip = getIpAddress(domain);

  if (ip.empty()){
    LOG_ET("Ten mien [%s] khong ton tai. Error:%d\n",domain.c_str(),WSAGetLastError());
    LOG_D("");
    return;
  }
  
  LOG_WT("IPv4 cua ten mien [%s] => [%s] \n",domain.c_str(),ip.c_str());
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
  showUri("http://www.hdvietnam.com");  

  LOG_D("");
}
int main(int argc, char const *argv[])
{
  LOG_WT("*** NETWORK HTTP CLIENT ***\n");

  //test_uri();

  LOG_IT("[*] Khoi tao WinSocket\n");
  if (!InitWinSock()) return -1;

  //showUri("http://danang.edu.vn/documents/11834/0/NAMHOC2023-2024.png");
  //test_uri();
  //return -1;
  //test_getIpAddress();

  //std::string url = "http://tuyensinh.husc.edu.vn/category/quyche/";
   // std::string url = "	http://vnpay.vn/";
   // std::string url = " http://thongtin24h.net/";
    std::string url = " http://www.hdvietnam.com/";
  showUri(url);
  Uri u = Uri::Parse(url);
  
  //Tạo thư mục có tên như domain (u.Host) để lưu nội dung
  CreateDirectoryA(u.Host.c_str(),NULL);
  //Lưu nội dung trang chủ vào file index.html trong u.Host
  saveHost2Html(u.Host.c_str(),u.getPort(),"/",u.Host + "/index.html");
  //Lưu nội dung url vào file hdvietnam.html trong u.Host
  saveHost2Html(u.Host.c_str(),u.getPort(),u.getPath(),u.Host + "/hdvietnam.html");

  //Clean Winsock trước khi thoát ứng dụng
  WSACleanup();
  LOG_D("");
  return 0;
}