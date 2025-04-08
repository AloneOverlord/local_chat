#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<string>
#include<thread>
#pragma comment(lib,"ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024
using namespace std;
//��д������Ϣ���̺߳���
void recv_thread(SOCKET client_socket) {
	
		char buffer[BUFFER_SIZE] = { 0 };
		while (true) {
			int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
			if (recv_len <= 0) {
				cerr << "�ͻ��˶Ͽ�����orδ������Ϣ\n";
				break;
			}
			buffer[recv_len] = '\0';
			cout << "�ͻ��ˣ�" << buffer << endl;
			
		}
		closesocket(client_socket);
	
}
// ��д���Է�����Ϣ���̺߳���
//�����������������߳�
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
	//��wsa
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//����������׽���
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	//�����������˿ڰ�
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;//�������е�ַ
	bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	//��������
	listen(server_socket, 3);//���ȴ���������3
	cout << "��������������\n";
	//��ʼѭ������accept
	
	//����ֱ�ӿ����������̣�
	thread accept_th(accept_thread,server_socket);
	accept_th.detach();
	// ���̵߳ȴ��˳�����
	cout << "���� 'exit' ֹͣ������...\n";
	string cmd;
	while (true) {
		cin >> cmd;
		if (cmd == "exit") break;
	}
	closesocket(server_socket);
	WSACleanup();
}