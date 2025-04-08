#include<iostream>
#include<WinSock2.h>
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
	WSADATA wsaData;
	SOCKET sock;
	sockaddr_in serverAddr;
	char buffer[BUFFER_SIZE] = { 0 };
	//初始化WSA
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "client wsastartup fail\n";
		return -1;
	}
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "sock fail\n";
		WSACleanup();
		return -1;
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	//链接服务器
	if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "server connect fail\n";
		closesocket(sock);
		WSACleanup();
		return -1;
	}
	string message = "hello,server";
	send(sock, message.c_str(), message.length(), 0);

	int bytesRead = recv(sock, buffer, BUFFER_SIZE, 0);
	if (bytesRead > 0) {
		cout << "server:" << buffer << endl;
	}
	closesocket(sock);
	WSACleanup();
	system("pause");
	return 0;
}
