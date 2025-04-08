#include<iostream>
#include<string>
#include<WS2tcpip.h>
#include<thread>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PORT 8080
#define BUFFER_SIZE 1024

SOCKET client_socket;//方便后续子线程函数调用
//客户端接受函数
void Receive_message() {
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		//循环接受消息
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if (recv_len <= 0) {
			cerr << "client recv fail\n";
			break;
		}
		//如果收到信息
		buffer[recv_len] = '\0';
		cout << "服务端说：" << buffer << endl;
	}

}

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//判断暂时不做
	//创建客户端socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	//创建服务端的地址，并且连接
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	connect(client_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	cout << "连接服务器成功\n";
	//启动接受线程
	thread recv_thread(Receive_message);
	string message;
	while (true) {
		cout << "输入信息";
		getline(cin, message);
		send(client_socket, message.c_str(), message.size(), 0);
	}
	closesocket(client_socket);
	WSACleanup();

}