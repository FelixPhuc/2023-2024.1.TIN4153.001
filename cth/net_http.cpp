#include <cstdint>
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

//Hàm lưu URL vào FILE
bool save2FILE(const std::string& host,int port,std::string path, FILE* f){
  std::string ip = getIpAddress(host);
  SOCKET sockConnect;
  sockConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockConnect == INVALID_SOCKET) {
    //LOG_ET("socket() loi: %ld\n", WSAGetLastError());
    return false;
  }

  sockaddr_in srvService;
  srvService.sin_family = AF_INET;
  //srvService.sin_addr.s_addr = inet_addr(ip.c_str());
  inet_pton(AF_INET,ip.c_str(),&srvService.sin_addr.s_addr);
  srvService.sin_port = htons(port);
  
  // Connect to server.
  int iResult = connect(sockConnect, (struct sockaddr *) &srvService, sizeof (srvService));
  if (iResult == SOCKET_ERROR) {
    //LOG_ET("connect() error: %ld\n", WSAGetLastError());
    closesocket(sockConnect);
    return false;
  }
  if (path.empty()) path = "/";
  //std::string request = "GET / HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
  std::string request = StringFormat("GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",path.c_str(),host.c_str());
  if(send(sockConnect, request.c_str(), strlen(request.c_str())+1, 0) < 0){
    //LOG_ET("send() failed: %ld\n",WSAGetLastError());
  }
  
  std::string header;
  char buffer[4097];
  int n, total = 0, bytes = 0;
  std::string raw_site;
  bool isHeader_completed = false;
  uint64_t Content_Length = 0;
  int val_tmp =0;
  while((n = recv(sockConnect, buffer, 4096, 0)) > 0){
    total += n;

    if (!isHeader_completed){ //nhận Response Header
      
      buffer[n] = 0;
      
      std::string t(buffer);
      size_t pos = t.find("\r\n\r\n");//4 kytu 
      if (pos != std::string::npos){
        isHeader_completed = true;

        header.append(t.substr(0,pos));
        bytes += (total - ((int)pos + 4));
        size_t len_at = header.find("Content-Length:");
        if (len_at != std::string::npos){
          Content_Length = strtoull(&header[len_at + 16], NULL, 10);
        }
        LOG_D("[HEADER ]==========>\n%s\n",header.c_str());
        int size_header = (int)header.size();
        LOG_W("HEADER SIZE:%d bytes:%d Total:%d Content_Length:%llu\n",size_header,bytes,total,Content_Length);
        if (f) fwrite (&buffer[pos + 4] , sizeof(char), (size_t)bytes, f);
      }
      else{
        header.append(buffer);
      }            
    }
    else{
      bytes += n;
      if (f) fwrite (buffer , sizeof(char), (size_t)n, f);
      if (Content_Length > 0) {
        double val = 100 * (double)bytes/Content_Length;  
        int t = (int)val;              
        if (val_tmp != t) LOG_DT("Downloading %0.1lf%% ...\n",val);
        val_tmp = t;
        
      }
    }
  }//while((n = recv(sockConnect, buffer, 4096, 0)) > 0){

  closesocket(sockConnect);
  
  LOG_WT("IMAGE SIZE:%d/%d\n",bytes,total);
  return true;
}

//Hàm lưu URL vào file
void saveURL2File(const std::string& urlJPG, const std::string& file){
  //thử tìm hiểu đoạn code để save file ảnh
  LOG_I("Bài tập: Lưu file ảnh từ website\n");
  LOG_I("URL [%s]\n",urlJPG.c_str());  
  LOG_E("1. Tìm hiểu các tài liệu về lập trình lưu file JPG (nhị phân) trên google\n");
  LOG_E("2. Giải thích cách làm trong file WORD và đưa lên github trong thư mục của từng thành viên\n");

  FILE *f = fopen(file.c_str(),"wb");

  Uri u = Uri::Parse(urlJPG);
  save2FILE(u.Host.c_str(),u.getPort(),u.getPath(),f);

  fclose(f);
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

  LOG_D("");
}
int main(int argc, char const *argv[])
{
  LOG_WT("*** NETWORK HTTP CLIENT ***\n");

  //test_uri();

  LOG_IT("[*] Khoi tao WinSocket\n");
  if (!InitWinSock()) return -1;
  std::string url;
  Uri u;
  
/* 
  //url = "http://tuyensinh.husc.edu.vn/category/quyche/";
  url  = "http://danang.edu.vn/van-ban-chi-dao";
  showUri(url);
  u = Uri::Parse(url);
  
  //Tạo thư mục có tên như domain (u.Host) để lưu nội dung
  CreateDirectoryA(u.Host.c_str(),NULL);
  //Lưu nội dung trang chủ vào file index.html trong u.Host
  saveHost2Html(u.Host.c_str(),u.getPort(),"/",u.Host + "/index.html");
  //Lưu nội dung url vào file quyche.html trong u.Host
  saveHost2Html(u.Host.c_str(),u.getPort(),u.getPath(),u.Host + "/quyche.html");
*/

  //url = "http://iuh.edu.vn/Resource/Upload2/Image/album/toan%20canh%20xl.JPG";
  //url = "http://oj.husc.edu.vn/";
  //url = "http://daotao.hutech.edu.vn/Upload/file/HuongDanHuyHP/HUONG%20DAN%20HUY%20HP%202022.doc.docx";
  url = "daotao.hutech.edu.vn/Upload/file/huong%20dan%20dang%20ky%20mon%20hoc%20Video%20web.wmv";
  u = Uri::Parse(url);
  //Tạo thư mục có tên như domain (u.Host) để lưu nội dung
  CreateDirectoryA(u.Host.c_str(),NULL);

  //showUri("http://iuh.edu.vn/Resource/Upload2/Image/album/toan%20canh%20xl.JPG");
  //showUri(url);
  
  std::string fname = "image.jpg";
  size_t pos = url.rfind("/");
  if (pos != std::string::npos){
    fname = url.substr(pos + 1);
    fname = std::regex_replace(fname, std::regex("%20"), "_");
    //std::replace( fname.begin(),fname.end(), (char)0x20, '_');
  }
  
  saveURL2File(url,  u.Host + "/" + fname);  

  //Clean Winsock trước khi thoát ứng dụng
  WSACleanup();
  LOG_D("");
  return 0;
}