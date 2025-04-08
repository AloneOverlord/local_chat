#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PORT 8080
int main() {
	//window��Ҫ��socket
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "wsaData fail" <<endl;
		return -1;
	}
	//����socket����ʼ��
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		cerr << "initialize socket fail\n";
		WSACleanup();
		return -1;
	}
	//��socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "bind fail\n";
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	//����socket
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
		cerr << "listen fail\n";
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	cout << "���ڼ���8080�˿�\n";
	//��������
	SOCKET client_scoket;
	sockaddr_in clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	client_scoket = accept(server_socket, (sockaddr*)&clientAddr, &clientAddrLen);
	if (client_scoket == INVALID_SOCKET) {
		cerr << "accept error\n";
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	cout << "���ӳɹ�\n";
	char buffer[1024] = { 0 };
	int bytesReceived = recv(client_scoket, buffer, sizeof(buffer), 0);
	if (bytesReceived > 0) {
		cout << "�յ�����" << buffer << endl;
		send(client_scoket, "hello client", 12, 0);
	}
	closesocket(client_scoket);
	closesocket(server_socket);
	WSACleanup();
	system("pause");

}