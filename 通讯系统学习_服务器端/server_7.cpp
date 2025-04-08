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
			cerr << "�ͻ��˶Ͽ�����\n";
			break;
		}
		buffer[recv_len] = '\0';
		cout << "�ͻ��ˣ�" << buffer << endl;
	}
	closesocket(client_socket);
}

void acpt_thread(SOCKET server_socket) {
	while (true) {
		
		sockaddr_in clientAddr = {};
		int client_len = sizeof(clientAddr);
		SOCKET client_socket = accept(server_socket, (sockaddr*)&clientAddr, &client_len);
		if (client_socket == SOCKET_ERROR) {
			cerr << "accept����ʧ��\n";
			continue;
		}
		cout << "accept���ӳɹ�\n";
		//����͵��ý�����Ϣ�ĺ���
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
	cout << "�������״̬\n";
	//���ܿͻ�������	���������߳�ģʽȥ����
	thread acpt_th(acpt_thread, server_socket);
	acpt_th.detach();
	//���߸����Ƿ�رշ�����
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