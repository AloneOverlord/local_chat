#include<iostream>
#include<string>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<thread>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

//这里写线程函数为后面服务端发消息准备


//客户端接受函数的线程
void recv_thread(SOCKET client_sock) {
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		int rec_len = recv(client_sock, buffer, BUFFER_SIZE, 0);
		if (rec_len <= 0) {
			cerr << "未连接\n";
			return;
		}
		buffer[rec_len] = '\0';
		
		cout << "客户端" << buffer << endl;
	}
	closesocket(client_sock);
}
int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建服务器socket
	SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
	//创建地址绑定
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(server_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
	listen(server_sock, 3);
	cout << "服务器进入监听状态\n";
	//接受客户端连接	错误：我觉得这一步应该放在线程里操作，这样可以接受多个客户端
	//用while循环接受客户端
	while (true) {
		sockaddr_in clientAddr{};
		int clientAddr_len = sizeof(clientAddr);
		SOCKET client_socket = accept(server_sock, (sockaddr*)&clientAddr,&clientAddr_len);
		if (client_socket == INVALID_SOCKET) {
			cerr << "fail accept\n";
			continue;
		}
		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientAddr.sin_addr, client_ip, sizeof(client_ip));
		cout << "客户端IP" << client_ip << "port:" << ntohs(clientAddr.sin_port) << endl;
		
		thread rec_th(recv_thread, client_socket);
		rec_th.detach();
	}
	closesocket(server_sock);
	WSACleanup();
}