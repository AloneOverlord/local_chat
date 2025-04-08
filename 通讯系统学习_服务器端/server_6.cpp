#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<string>
#include<thread>
#pragma comment(lib,"ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024
using namespace std;
//编写接受信息的线程函数
void recv_thread(SOCKET client_socket) {
	
		char buffer[BUFFER_SIZE] = { 0 };
		while (true) {
			int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
			if (recv_len <= 0) {
				cerr << "客户端断开连接or未输入消息\n";
				break;
			}
			buffer[recv_len] = '\0';
			cout << "客户端：" << buffer << endl;
			
		}
		closesocket(client_socket);
	
}
// 编写可以发送信息的线程函数
//创建函数用来接收线程
void accept_thread(SOCKET server_socket) {
	while (true) {
		//SOCKET client_socket;
		sockaddr_in clientAddr = {};
		int clientAddr_len = sizeof(clientAddr);
		SOCKET client_socket = accept(server_socket, (sockaddr*)&clientAddr, &clientAddr_len);
		if (client_socket == SOCKET_ERROR) {
			cerr << "accept fail\n";
			continue;
		}
		cout << "accpet susscss!!!\n";
		thread recv_th(recv_thread, client_socket);
		recv_th.detach();
	}
}

int main() {
	//打开wsa
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建服务端套接字
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	//创建服务器端口绑定
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;//监听所有地址
	bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	//开启监听
	listen(server_socket, 3);//最大等待监听数量3
	cout << "服务器开启监听\n";
	//开始循环连接accept
	
	//这里直接开启三个进程？
	thread accept_th(accept_thread,server_socket);
	accept_th.detach();
	// 主线程等待退出命令
	cout << "输入 'exit' 停止服务器...\n";
	string cmd;
	while (true) {
		cin >> cmd;
		if (cmd == "exit") break;
	}
	closesocket(server_socket);
	WSACleanup();
}