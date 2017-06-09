#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <string>

#pragma comment(lib,"ws2_32.lib")

SOCKET CONNECTION;

void clientThread() {
	int BUFFERLENGTH;
	int RETVAL;
	while (1) {
		RETVAL = recv(CONNECTION, (char*)&BUFFERLENGTH, sizeof(int), NULL);
		if (RETVAL == SOCKET_ERROR) {
			std::cout << "CLIENT::ERROR::Lost connection to the server" << std::endl;
			closesocket(CONNECTION);
			WSACleanup();
			break;
		}
		char* BUFFER = new char[BUFFERLENGTH + 1];
		BUFFER[BUFFERLENGTH] = '\0';
		RETVAL = recv(CONNECTION, BUFFER, BUFFERLENGTH, NULL);
		if (RETVAL == SOCKET_ERROR) {
			std::cout << "CLIENT::ERROR::Lost connection to the server" << std::endl;
			closesocket(CONNECTION);
			WSACleanup();
			break;
		}
		std::cout << BUFFER << std::endl;
		delete[] BUFFER;
	}
}

int main(int argc, char *argv[]) {

	WSAData WSADATA;
	WORD DLLVERSION = MAKEWORD(2, 1);

	if (WSAStartup(DLLVERSION, &WSADATA) != 0) {
		std::cout << "CLIENT::ERROR::WSAStartup failed\n";
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "CLIENT::WSAStartup successful\n";
	}

	SOCKADDR_IN ADDR;
	
	ADDR.sin_addr.s_addr = inet_addr("127.0.0.1");
	ADDR.sin_port = htons(8899);
	ADDR.sin_family = AF_INET;

	CONNECTION = socket(AF_INET, SOCK_STREAM, NULL);
	int ADDRLEN = sizeof(ADDR);
	if (connect(CONNECTION, (SOCKADDR*)&ADDR, ADDRLEN) != 0) {
		std::cout << "CLIENT::ERROR::Failed to connect\n";
		WSACleanup();
		return 1;
	}
	else {
		std::thread t = std::thread(clientThread);
		t.detach();
	}

	while (1) {
		std::string MESSAGE;
		std::getline(std::cin, MESSAGE);
		int MESSAGELEN = MESSAGE.size();
		send(CONNECTION, (char*)&MESSAGELEN, sizeof(int), NULL);
		send(CONNECTION, MESSAGE.c_str(), MESSAGELEN, NULL);
	}

	return 0;
}
