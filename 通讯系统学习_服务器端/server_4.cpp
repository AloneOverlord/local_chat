#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

//�̺߳���
DWORD WINAPI clientHandler(LPVOID lpPragm) {
	//�̺߳�����Ҫ�����յ���Ϣ recv
	//Ҫ������Ϣ-1���пͻ���socket���ͻ���socke���̺߳���lpPragm��ʼ��
	SOCKET client_socket = *(SOCKET*)lpPragm;
	//���̺߳����������Ϣ�����ͻ���socket
	//ͬʱ������ϢҪ��һ���ֽڻ�����
	char buffer[BUFFER_SIZE] = { 0 };
	while (true) {
		int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
		//�����Ƿ��յ���Ϣ	�յ���Ϣ�Ļ�recv_len�Ͳ�Ϊ0
		if (recv_len <= 0) {
			cerr << "recv fail\n";
			break;
		}
		//�յ���Ϣ�Ļ�recv_len��Ϊ�� c�������ĩβ��ӿ��ַ���ʾ����
		buffer[recv_len] = '\0';
		cout << "�ͻ��ˣ�" << buffer << endl;
		string message;
		cout << "��������������Ϣ��";
		getline(cin, message);
		send(client_socket, message.c_str(), message.size(), 0);
	}
	closesocket(client_socket);
	return 0;
}


int main() {
	//socketҪ����WSA
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "wsastartup fail\n";
		return -1;
	}
	//����������socket    �����INVALID_SOCKET
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

	//��
		//��֮ǰҪ���õ�ַ��
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);//�����ת��
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;//��������IP
	bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	//����Ƿ�󶨳ɹ���SOCK_ERROR

	//����
	listen(server_socket, 3);//3���߳�
	cout << "��������" << PORT << endl;

	//ѭ�����ܿͻ�������
	while (true) {
		//���յ��ǿͻ���socket
		SOCKET client_socket = accept(server_socket, nullptr, nullptr);

		//���һ���Ƿ���ܳɹ� �ɹ��˾ʹ����߳�ȥ������Ϣ
		if (client_socket == INVALID_SOCKET) {
			cerr << "accept error\n";
			continue;
		}
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