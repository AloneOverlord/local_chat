#include<iostream>
#include<string>
#include<thread>
#include<WinSock2.h>
#include<mutex>
#pragma comment(lib,"ws2_32.lib")
#define PORT 8080
#define BUFFER_SIZE 1024
using namespace std;
mutex g_lock;
void recv_thread(SOCKET client_socket,u_short port) {
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if (recv_len <= 0) {
			{
			lock_guard<mutex> lock(g_lock);
			cerr << "客户端" << port << "断开\n";
			}
			break;
		}
		buffer[recv_len] = '\0';
		{
		lock_guard<mutex> lock(g_lock);
		cout << "客户端" << port << ":" << buffer << endl;
		}
	}
	closesocket(client_socket);
}
void acpt_thread(SOCKET server_socket) {
	while (true) {
		sockaddr_in clientAddr = {};
		int cliAddr_len = sizeof(clientAddr);
		SOCKET client_socket = accept(server_socket, (sockaddr*)&clientAddr, &cliAddr_len);
		if (client_socket == INVALID_SOCKET) {
			{
			lock_guard<mutex> lock(g_lock);
			cerr << "接受失败\n";
			}
			continue;
		}
		auto port = ntohs(clientAddr.sin_port);
		{
			lock_guard<mutex> lock(g_lock);
			cout << "接受成功客户端：" << port << endl;
		}
		//进入接受信息线程
		thread recv_th(recv_thread, client_socket,port);
		recv_th.detach();
	}
}

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	listen(server_socket, 3);
	cout << "进入监听状态" << endl;
	//接受连接
	thread acpt_th(acpt_thread, server_socket);
	acpt_th.detach();
	string overmain;
	while (true) {
		getline(cin, overmain);
		if (overmain == "exit") {
			break;
		}
	}
	closesocket(server_socket);
	WSACleanup();
	return 0;
}