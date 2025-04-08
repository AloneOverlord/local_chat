#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

//线程函数
DWORD WINAPI clientHandler(LPVOID lpPragm) {
	//线程函数主要负责收到消息 recv
	//要接受信息-1，有客户端socket，客户端socke由线程函数lpPragm初始化
	SOCKET client_socket = *(SOCKET*)lpPragm;
	//把线程函数传入的信息传给客户端socket
	//同时接受消息要有一个字节缓冲区
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		//检验是否收到消息	收到消息的话recv_len就不为0
		if (recv_len <= 0) {
			cerr << "recv fail\n";
			break;
		}
		//收到信息的话recv_len不为空 c风格中在末尾添加空字符表示结束
		buffer[recv_len] = '\0';
		cout << "客户端：" << buffer << endl;
		string message;
		cout << "服务器请输入消息：";
		getline(cin, message);
		send(client_socket, message.c_str(), message.size(), 0);
	}
	closesocket(client_socket);
	return 0;
}


int main() {
	//socket要启动WSA
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "wsastartup fail\n";
		return -1;
	}
	//创建服务器socket    检测用INVALID_SOCKET
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

	//绑定
		//绑定之前要给好地址蔟
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);//大段序转换
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;//监听任意IP
	bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	//检测是否绑定成功用SOCK_ERROR

	//监听
	listen(server_socket, 3);//3个线程
	cout << "开启监听" << PORT << endl;

	//循环接受客户端连接
	while (true) {
		//接收的是客户端socket
		SOCKET client_socket = accept(server_socket, nullptr, nullptr);

		//检测一下是否接受成功 成功了就创建线程去接受信息
		if (client_socket == INVALID_SOCKET) {
			cerr << "accept error\n";
			continue;
		}
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
	return 0;

}