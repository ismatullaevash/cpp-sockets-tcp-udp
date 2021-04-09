#include <iostream>
#include <string>
#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "Could not start DLLs" << std::endl;
		return 0;
	}
	std::string IP, method;
	int portNum;
	std::cout << "Please enter the method(TCP/UDP)" << std::endl;
	std::getline(std::cin, method);
	SOCKET ClientSocket;
	if (method == "TCP" || method == "tcp") {
		ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	else if (method == "UDP" || method == "udp") {
		ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	if (ClientSocket == INVALID_SOCKET) {
		std::cout << "Could not create socket" << std::endl;
		WSACleanup();
		return 0;
	}
	std::cout << "Please enter the IP address" << std::endl;
	std::getline(std::cin, IP);
	std::cout << "Please enter the port number" << std::endl;
	std::cin >> portNum;
	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_port = htons(portNum);
	SvrAddr.sin_addr.s_addr = inet_addr(IP.c_str());
	if (method == "TCP" || method == "tcp") {
		if ((connect(ClientSocket, (struct sockaddr*) & SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
			std::cout << "Failed to connect to server" << std::endl;
			closesocket(ClientSocket);
			WSACleanup();
			return 0;
		}
	}
	char TxBuffer[128] = {};
	char RxBuffer[128] = {};
	std::cout << "Enter a String to transmit" << std::endl;
	std::cin >> TxBuffer;
	if (method == "TCP" || method == "tcp") {
		send(ClientSocket, TxBuffer, sizeof(TxBuffer), 0);
		recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
	}
	else {
		sendto(ClientSocket, TxBuffer, sizeof(TxBuffer), 0,
			(struct sockaddr*) & SvrAddr, sizeof(SvrAddr));
		int addr_len = sizeof(SvrAddr);
		recvfrom(ClientSocket, RxBuffer, sizeof(RxBuffer), 0,
			(struct sockaddr*) & SvrAddr, &addr_len);
	}
	std::cout << "Response: " << RxBuffer << std::endl;
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}