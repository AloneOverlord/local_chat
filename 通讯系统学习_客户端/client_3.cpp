#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<thread>
#include<string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"   //������IP��ַ

SOCKET client_socket;//�������̷߳���

//������Ϣ���̺߳���
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
		cout << "��������" << buffer << endl;
		cout << "��������Ϣ" << endl;
		flush(cout);
	}
}
int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	//�ж���ʱ��д   ��INVALID_SOCKET�ж�
	sockaddr_in serverAddr;//�����¼���Ƿ�������IP��ַ
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

	//���ӷ�����
	connect(client_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	//��socket_error�ж�
	cout << "���ӳɹ�\n";
	//�����߳�
	thread recv_thread(ReceiveMessage);
	string message;
	while (true) {
		cout << "������Ϣ";
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