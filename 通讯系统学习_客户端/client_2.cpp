#include <iostream>
#include <winsock2.h>
#include<WS2tcpip.h>
#include <thread>  // C++11�߳̿�
#include<string>
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1" // �����IP�����ػ��ص�ַ��
#define PORT 8080
#define BUFFER_SIZE 1024

SOCKET client_socket; // ȫ�ֱ������������̷߳���

// ������Ϣ���̺߳���
void ReceiveMessages() {
    char buffer[BUFFER_SIZE] = { 0 };
    while (true) {
        // 1. ���շ������Ϣ�������������
        int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (recv_len <= 0) {
            std::cerr << "�����˶Ͽ����ӡ�" << std::endl;
            break;
        }
        buffer[recv_len] = '\0';
        std::cout << "\n�����˵: " << buffer << std::endl;
        std::cout << "������Ϣ: ";
        std::flush(std::cout); // ����ˢ�������ȷ����ʾ����ʾ
    }
}

int main() {
    // ��ʼ��Winsock��
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 1. �����ͻ���Socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "����Socketʧ�ܡ�" << std::endl;
        return 1;
    }

    // 2. ���ӷ����
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr); // IPתΪ�����Ƹ�ʽ

    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "���ӷ����ʧ�ܡ�" << std::endl;
        closesocket(client_socket);
        return 1;
    }
    std::cout << "�����ӵ�����ˣ�������Ϣ������ exit �˳���:" << std::endl;

    // 3. ���������߳�
    std::thread recv_thread(ReceiveMessages);

    // 4. ���߳�ѭ����ȡ���벢����
    std::string message;
    while (true) {
        std::cout << "������Ϣ: ";
        std::getline(std::cin, message);
        if (message == "exit") {
            break;
        }
        send(client_socket, message.c_str(), message.size(), 0);
    }

    // ������Դ
    closesocket(client_socket);
    WSACleanup();
    return 0;
}
