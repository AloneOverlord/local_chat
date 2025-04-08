#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<thread>
#include<string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"   //服务器IP地址

SOCKET client_socket;//用于子线程访问

//接受消息的线程函数
void ReceiveMessage() {
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if (recv_len <= 0)
		{
			cerr << "client recv_len<=0\n";
			break;
		}
		buffer[recv_len] = '\0';
		cout << "服务器：" << buffer << endl;
		cout << "请输入消息" << endl;
		flush(cout);
	}
}
int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	//判断暂时不写   用INVALID_SOCKET判断
	sockaddr_in serverAddr;//这里记录的是服务器的IP地址
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

	//连接服务器
	connect(client_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	//用socket_error判断
	cout << "连接成功\n";
	//启动线程
	thread recv_thread(ReceiveMessage);
	string message;
	while (true) {
		cout << "输入信息";
		getline(cin,message);
		if (message == "exit") {
			break;
		}
		send(client_socket, message.c_str(), message.size(), 0);
	}
	closesocket(client_socket);
	WSACleanup();
	return 0;



}