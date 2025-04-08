#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PORT 8080 //定义端口
#define BUFFER_SIZE 1024 //缓冲区域大小
//线程函数
DWORD WINAPI clientHandler(LPVOID lpPragm) {
		//线程中负责接受客户端的消息    发送消息后续应该也可以添加
		//创建客户端的socket
	SOCKET client_socket = *(SOCKET*)lpPragm;
	//初始化客户端接受消息的缓存区域
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		//接收客户端消息
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if (recv_len <= 0) {
			cerr << "未接收到客户端消息or断开连接\n";
			closesocket(client_socket);
			break;
		}
		//如果接受到消息了就输出出来	要输出之前要在buffer数组中添加文字的结束符号\0
		buffer[recv_len] = '\0';//这里recv_len刚好就是接受到的数据长度
		cout << "客户端：" << buffer << endl;
	}
	closesocket(client_socket);
	return 0;
}

int main() {
	//win上要使用socket需要先启动wsa服务
	WSADATA wsaData;
	//确定tcp协议的版本信息
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "wsastart fail\n";
		return -1;
	}
	//创建socket
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddr;//服务器的地址信息
	serverAddr.sin_family = AF_INET;//服务地址蔟
	serverAddr.sin_port = htons(PORT);//把端口转换为大段序
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;//监听所有的ip端口
	//绑定socket
	if (bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "bind fail\n";
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	//监听
	if (listen(server_socket, 3) == SOCKET_ERROR) {
		cerr << "listen fail\n";
		return -1;
	}
	cout << "服务器开启监听状态\n";
	//循环接收客户端连接
	while (true) {
		SOCKET client_socket = accept(server_socket, nullptr, nullptr);
		if (client_socket == INVALID_SOCKET) {
			cerr << "accept fail\n";
			continue;
		}
		cout << "client accept success\n";
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
	return 0;
}