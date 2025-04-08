#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PORT 8080//定义端口
#define BUFFER_SIZE 1024//缓冲空间大小

DWORD WINAPI clientHandler(LPVOID lpParam) {
	//获取客户端socket
	SOCKET client_socket = *(SOCKET*)lpParam;
	//初始化缓冲区间
	char buffer[BUFFER_SIZE] = { 0 };
	//循环接受客户端
	while (true) {
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if (recv_len <= 0) {
			//接收失败
			cerr << "client recv fail or error\n";
			break;
		}
		//收到消息的话
		buffer[recv_len] = '\0';//字符串末尾添加空字符代表结束
		cout << "client:" << buffer << endl;
	}
	closesocket(client_socket);
	return 0;
}
int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建服务端的socket
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		cerr << "create server_socket fail\n";
		WSACleanup();
		return -1;
	}
	//绑定socket和ip
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
		//开始绑定bind
	if (bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "bind fail\n";
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	//监听阶段
	if (listen(server_socket, 3) == SOCKET_ERROR) {
		cerr << "listen fail\n";
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	//简单提示
	cout << "服务器已经进入监听状态\n";

	//循环接受客户端连接
	while (true) {
		//创建客户端socket
		SOCKET client_socket = accept(server_socket, nullptr, nullptr);
		if (client_socket == INVALID_SOCKET) {
			cerr << "接受连接失败\n";
			continue;
		}
		cout << "client 已连接\n";
		
		//创建线程
		CreateThread(
			nullptr,
			0,
			clientHandler,
			&client_socket,
			0,
			nullptr
		);
	}
	closesocket(server_socket);
	WSACleanup();

	system("pause");

}
