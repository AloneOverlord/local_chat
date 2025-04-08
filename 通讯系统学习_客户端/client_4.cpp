#include<iostream>
#include<string>
#include<WS2tcpip.h>
#include<thread>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PORT 8080
#define BUFFER_SIZE 1024

SOCKET client_socket;//����������̺߳�������
//�ͻ��˽��ܺ���
void Receive_message() {
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		//ѭ��������Ϣ
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if (recv_len <= 0) {
			cerr << "client recv fail\n";
			break;
		}
		//����յ���Ϣ
		buffer[recv_len] = '\0';
		cout << "�����˵��" << buffer << endl;
	}

}

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//�ж���ʱ����
	//�����ͻ���socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	//��������˵ĵ�ַ����������
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	connect(client_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	cout << "���ӷ������ɹ�\n";
	//���������߳�
	thread recv_thread(Receive_message);
	string message;
	while (true) {
		cout << "������Ϣ";
		getline(cin, message);
		send(client_socket, message.c_str(), message.size(), 0);
	}
	closesocket(client_socket);
	WSACleanup();

}