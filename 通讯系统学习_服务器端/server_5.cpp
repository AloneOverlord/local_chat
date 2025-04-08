#include<iostream>
#include<string>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<thread>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

//����д�̺߳���Ϊ�������˷���Ϣ׼��


//�ͻ��˽��ܺ������߳�
void recv_thread(SOCKET client_sock) {
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		int rec_len = recv(client_sock, buffer, BUFFER_SIZE, 0);
		if (rec_len <= 0) {
			cerr << "δ����\n";
			return;
		}
		buffer[rec_len] = '\0';
		
		cout << "�ͻ���" << buffer << endl;
	}
	closesocket(client_sock);
}
int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//����������socket
	SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
	//������ַ��
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(server_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
	listen(server_sock, 3);
	cout << "�������������״̬\n";
	//���ܿͻ�������	�����Ҿ�����һ��Ӧ�÷����߳���������������Խ��ܶ���ͻ���
	//��whileѭ�����ܿͻ���
	while (true) {
		sockaddr_in clientAddr{};
		int clientAddr_len = sizeof(clientAddr);
		SOCKET client_socket = accept(server_sock, (sockaddr*)&clientAddr,&clientAddr_len);
		if (client_socket == INVALID_SOCKET) {
			cerr << "fail accept\n";
			continue;
		}
		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientAddr.sin_addr, client_ip, sizeof(client_ip));
		cout << "�ͻ���IP" << client_ip << "port:" << ntohs(clientAddr.sin_port) << endl;
		
		thread rec_th(recv_thread, client_socket);
		rec_th.detach();
	}
	closesocket(server_sock);
	WSACleanup();
}