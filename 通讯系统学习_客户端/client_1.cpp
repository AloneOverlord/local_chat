#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;

int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "wsastartup fail\n";
		return -1;
	}
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

	if (connect(client_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "connet fail\n";
		closesocket(client_socket);
		WSACleanup();
		return -1;
	}
	send(client_socket, "hello server", 12, 0);
	char buffer[1024] = { 0 };
	recv(client_socket, buffer, sizeof(buffer), 0);
	cout << "server anser:" << buffer << endl;
	closesocket(client_socket);
	WSACleanup();
	system("pause");
	return 0;

}