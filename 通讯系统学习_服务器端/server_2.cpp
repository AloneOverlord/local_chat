#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PORT 8080//����˿�
#define BUFFER_SIZE 1024//����ռ��С

DWORD WINAPI clientHandler(LPVOID lpParam) {
	//��ȡ�ͻ���socket
	SOCKET client_socket = *(SOCKET*)lpParam;
	//��ʼ����������
	char buffer[BUFFER_SIZE] = { 0 };
	//ѭ�����ܿͻ���
	while (true) {
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if (recv_len <= 0) {
			//����ʧ��
			cerr << "client recv fail or error\n";
			break;
		}
		//�յ���Ϣ�Ļ�
		buffer[recv_len] = '\0';//�ַ���ĩβ��ӿ��ַ��������
		cout << "client:" << buffer << endl;
	}
	closesocket(client_socket);
	return 0;
}
int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//��������˵�socket
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		cerr << "create server_socket fail\n";
		WSACleanup();
		return -1;
	}
	//��socket��ip
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
		//��ʼ��bind
	if (bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "bind fail\n";
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	//�����׶�
	if (listen(server_socket, 3) == SOCKET_ERROR) {
		cerr << "listen fail\n";
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	//����ʾ
	cout << "�������Ѿ��������״̬\n";

	//ѭ�����ܿͻ�������
	while (true) {
		//�����ͻ���socket
		SOCKET client_socket = accept(server_socket, nullptr, nullptr);
		if (client_socket == INVALID_SOCKET) {
			cerr << "��������ʧ��\n";
			continue;
		}
		cout << "client ������\n";
		
		//�����߳�
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

}
