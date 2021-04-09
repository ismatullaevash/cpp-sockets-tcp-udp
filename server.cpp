#include <iostream>
#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

int main() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Could not start DLLs" << std::endl;
        return 0;
    }

    SOCKET ListenSocket;
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        std::cout << "Could not create socket" << std::endl;
        WSACleanup();
        return 0;
    }

    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    SvrAddr.sin_port = htons(27000);
    if (bind(ListenSocket, (struct sockaddr*) & SvrAddr,
        sizeof(SvrAddr)) == SOCKET_ERROR) {
        std::cout << "Could not bind socket to address" << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 0;
    }

    if (listen(ListenSocket, 1) == SOCKET_ERROR) {
        std::cout << "Could not start to listen" << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 0;
    }

    while (true) {
        std::cout << "Waiting for client connection" << std::endl;

        SOCKET ClientSocket;
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            std::cout << "Failed to accept connection" << std::endl;
            closesocket(ListenSocket);
            WSACleanup();
            return 0;
        }

        std::cout << "Connection Established" << std::endl;

        while (true) {
            char RxBuffer[128] = {};
            recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
            std::cout << "Msg Rx: " << RxBuffer << std::endl;

            char TxBuffer[128] = "Thanks for your message!";
            send(ClientSocket, TxBuffer, sizeof(TxBuffer), 0);

            if (strcmp(RxBuffer, "[q]") == 0) {
                std::cout << "Terminated by the user" << std::endl;
                break;
            }
        }
        closesocket(ClientSocket);
    }
    closesocket(ListenSocket);
    WSACleanup();

    return 0;
}