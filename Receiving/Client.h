#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Receiver.h"
#include "PrintReceiver.h"
#include "BufferedPrintReceiver.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

using namespace std;

class Client
{
public:
	Client(Receiver* recv);
	~Client();

	int connectToServer(char* serveraddress, char* port = DEFAULT_PORT);
	bool isConnected();
	int sendbuf(char* buf, int buflen = DEFAULT_BUFLEN);
	int receivebuf();
	char* getReceivebuf();
	int shutdownConnection();

private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	bool connected = false;
	char recvbuf[DEFAULT_BUFLEN];
	Receiver* receiver;
};

