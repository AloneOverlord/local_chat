#include<iostream>
#include<WinSock2.h>
#include<thread>
#pragma comment(lib,"ws2_32.lib")
#define PORT 8080
#define BUFFER_SIZE 1024
using namespace std;

void recv_thread(SOCKET client_socket) {
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if (recv_len <= 0) {
			cerr << "客户端断开连接\n";
			break;
		}
		buffer[recv_len] = '\0';
		cout << "客户端：" << buffer << endl;
	}
	closesocket(client_socket);
}

void acpt_thread(SOCKET server_socket) {
	while (true) {
		
		sockaddr_in clientAddr = {};
		int client_len = sizeof(clientAddr);
		SOCKET client_socket = accept(server_socket, (sockaddr*)&clientAddr, &client_len);
		if (client_socket == SOCKET_ERROR) {
			cerr << "accept连接失败\n";
			continue;
		}
		cout << "accept连接成功\n";
		//这里就调用接受信息的函数
		thread recv_th(recv_thread, client_socket);
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
	cout << "进入监听状态\n";
	//接受客户端连接	这里用子线程模式去运行
	thread acpt_th(acpt_thread, server_socket);
	acpt_th.detach();
	//主线负责是否关闭服务器
	string exit_ma;
	while (true) {
		cin >> exit_ma;
		if (exit_ma == "exit") {
			break;
		}
	}
	closesocket(server_socket);
	WSACleanup();
}