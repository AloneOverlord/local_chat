#include <iostream>
#include <winsock2.h>
#include<WS2tcpip.h>
#include <thread>  // C++11线程库
#include<string>
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1" // 服务端IP（本地环回地址）
#define PORT 8080
#define BUFFER_SIZE 1024

SOCKET client_socket; // 全局变量，用于子线程访问

// 接收消息的线程函数
void ReceiveMessages() {
    char buffer[BUFFER_SIZE] = { 0 };
    while (true) {
        // 1. 接收服务端消息（会阻塞在这里）
        int recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (recv_len <= 0) {
            std::cerr << "与服务端断开连接。" << std::endl;
            break;
        }
        buffer[recv_len] = '\0';
        std::cout << "\n服务端说: " << buffer << std::endl;
        std::cout << "输入消息: ";
        std::flush(std::cout); // 立即刷新输出，确保提示符显示
    }
}

int main() {
    // 初始化Winsock库
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 1. 创建客户端Socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "创建Socket失败。" << std::endl;
        return 1;
    }

    // 2. 连接服务端
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr); // IP转为二进制格式

    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "连接服务端失败。" << std::endl;
        closesocket(client_socket);
        return 1;
    }
    std::cout << "已连接到服务端！输入消息（输入 exit 退出）:" << std::endl;

    // 3. 启动接收线程
    std::thread recv_thread(ReceiveMessages);

    // 4. 主线程循环读取输入并发送
    std::string message;
    while (true) {
        std::cout << "输入消息: ";
        std::getline(std::cin, message);
        if (message == "exit") {
            break;
        }
        send(client_socket, message.c_str(), message.size(), 0);
    }

    // 清理资源
    closesocket(client_socket);
    WSACleanup();
    return 0;
}
