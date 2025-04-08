#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PORT 8080 //����˿�
#define BUFFER_SIZE 1024 //���������С
//�̺߳���
DWORD WINAPI clientHandler(LPVOID lpPragm) {
		//�߳��и�����ܿͻ��˵���Ϣ    ������Ϣ����Ӧ��Ҳ�������
		//�����ͻ��˵�socket
	SOCKET client_socket = *(SOCKET*)lpPragm;
	//��ʼ���ͻ��˽�����Ϣ�Ļ�������
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		//���տͻ�����Ϣ
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if (recv_len <= 0) {
			cerr << "δ���յ��ͻ�����Ϣor�Ͽ�����\n";
			closesocket(client_socket);
			break;
		}
		//������ܵ���Ϣ�˾��������	Ҫ���֮ǰҪ��buffer������������ֵĽ�������\0
		buffer[recv_len] = '\0';//����recv_len�պþ��ǽ��ܵ������ݳ���
		cout << "�ͻ��ˣ�" << buffer << endl;
	}
	closesocket(client_socket);
	return 0;
}

int main() {
	//win��Ҫʹ��socket��Ҫ������wsa����
	WSADATA wsaData;
	//ȷ��tcpЭ��İ汾��Ϣ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "wsastart fail\n";
		return -1;
	}
	//����socket
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddr;//�������ĵ�ַ��Ϣ
	serverAddr.sin_family = AF_INET;//�����ַ��
	serverAddr.sin_port = htons(PORT);//�Ѷ˿�ת��Ϊ�����
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;//�������е�ip�˿�
	//��socket
	if (bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "bind fail\n";
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	//����
	if (listen(server_socket, 3) == SOCKET_ERROR) {
		cerr << "listen fail\n";
		return -1;
	}
	cout << "��������������״̬\n";
	//ѭ�����տͻ�������
	while (true) {
		SOCKET client_socket = accept(server_socket, nullptr, nullptr);
		if (client_socket == INVALID_SOCKET) {
			cerr << "accept fail\n";
			continue;
		}
		cout << "client accept success\n";
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
	return 0;
}