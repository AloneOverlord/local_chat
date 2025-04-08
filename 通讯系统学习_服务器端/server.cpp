#include<iostream>
#include<WinSock2.h>
using namespace std;
//���ӿ����ʹ��socket
#pragma comment(lib,"ws2_32.lib")
#define PORT 8080//������˿�Ϊ������
#define BUFFER_SIZE 1024 //��������С

int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "WSAstartup fail" << endl;
		return -1;
	}
	//����socket
		SOCKET socket_server;
			//��ʼ��socket
		socket_server = socket(AF_INET, SOCK_STREAM, 0);
			//�����Ƿ��ʼ��socket�ɹ�
		if (socket_server == INVALID_SOCKET) {
		cerr << "initialize socket fail\n";
		WSACleanup();//��ʼ��ʧ���˾͹ر�����
		return -1;
		}
	//��socket
			//����һ��sockaddr_in ����
		sockaddr_in serverAddr;
			//��ֵ��sockaddr
		//��ַ��
		serverAddr.sin_family = AF_INET;
		//ip��ַ
		serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
		//�˿� htons�ǰɶ˿�ת��Ϊ�����
		serverAddr.sin_port = htons(PORT);
		//��
		if (bind(socket_server, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){
			cerr<<"bind error\n";
			closesocket(socket_server);
			WSACleanup();
			return -1;
		}
	//����socket
		if (listen(socket_server, 3) == SOCKET_ERROR) {
			cerr << "listen error\n";
			closesocket(socket_server);
			WSACleanup();
			return -1;
		}
		cout << "wait connect\n";
	//��������
		SOCKET socket_client;
		sockaddr_in clientAddr;
		int addrLen = sizeof(clientAddr);
		socket_client = accept(socket_server, (sockaddr*)&clientAddr, &addrLen);
		if (socket_client == INVALID_SOCKET) {
			cerr << "socket_client fail\n";
			closesocket(socket_server);
			WSACleanup();
			return -1;
		}
	//����(�ͻ��˵�)����
		char buffer[BUFFER_SIZE] = { 0 };
		int byteRead = recv(socket_client, buffer, BUFFER_SIZE, 0);
		if (byteRead > 0) {
			cout << "�ͻ��ˣ�" << buffer << endl;
		}
	//��������
		string response = "hello,client!";
		send(socket_client, response.c_str(), response.length(), 0);
	//�ر�socket
		closesocket(socket_client);
		closesocket(socket_server);
	//�ر�wsastartup
		WSACleanup();
		system("pause");
		
		return 0;
}