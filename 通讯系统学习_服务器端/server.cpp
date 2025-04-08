#include<iostream>
#include<WinSock2.h>
using namespace std;
//连接库才能使用socket
#pragma comment(lib,"ws2_32.lib")
#define PORT 8080//定义个端口为服务器
#define BUFFER_SIZE 1024 //缓冲区大小

int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "WSAstartup fail" << endl;
		return -1;
	}
	//创建socket
		SOCKET socket_server;
			//初始化socket
		socket_server = socket(AF_INET, SOCK_STREAM, 0);
			//检验是否初始化socket成功
		if (socket_server == INVALID_SOCKET) {
		cerr << "initialize socket fail\n";
		WSACleanup();//初始化失败了就关闭连接
		return -1;
		}
	//绑定socket
			//创建一个sockaddr_in 对象
		sockaddr_in serverAddr;
			//赋值给sockaddr
		//地址蔟
		serverAddr.sin_family = AF_INET;
		//ip地址
		serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
		//端口 htons是吧端口转换为大端序
		serverAddr.sin_port = htons(PORT);
		//绑定
		if (bind(socket_server, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){
			cerr<<"bind error\n";
			closesocket(socket_server);
			WSACleanup();
			return -1;
		}
	//监听socket
		if (listen(socket_server, 3) == SOCKET_ERROR) {
			cerr << "listen error\n";
			closesocket(socket_server);
			WSACleanup();
			return -1;
		}
		cout << "wait connect\n";
	//接收连接
		SOCKET socket_client;
		sockaddr_in clientAddr;
		int addrLen = sizeof(clientAddr);
		socket_client = accept(socket_server, (sockaddr*)&clientAddr, &addrLen);
		if (socket_client == INVALID_SOCKET) {
			cerr << "socket_client fail\n";
			closesocket(socket_server);
			WSACleanup();
			return -1;
		}
	//接收(客户端的)数据
		char buffer[BUFFER_SIZE] = { 0 };
		int byteRead = recv(socket_client, buffer, BUFFER_SIZE, 0);
		if (byteRead > 0) {
			cout << "客户端：" << buffer << endl;
		}
	//返回数据
		string response = "hello,client!";
		send(socket_client, response.c_str(), response.length(), 0);
	//关闭socket
		closesocket(socket_client);
		closesocket(socket_server);
	//关闭wsastartup
		WSACleanup();
		system("pause");
		
		return 0;
}