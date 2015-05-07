#include "Server.h"


Server::Server()
{
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
	}
}


Server::~Server()
{
}

int Server::connectToClient()
{
	char hostname[255];
	PHOSTENT hostinfo;
	char *IPAddress;

	if (gethostname(hostname, sizeof(hostname)) == 0)
	{
		if ((hostinfo = gethostbyname(hostname)) != NULL)
		{
			IPAddress = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			cout << "Ip-Adresse des Servers: " << IPAddress << endl;
		}
	}

	struct addrinfo *result = NULL;
	struct addrinfo hints;


	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	int iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	printf("Versucht Verbindung aufzubauen...");

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// No longer need server socket
	closesocket(ListenSocket);

	connected = true;

	printf("Verbindung aufgebaut!");

	return 0;
}

bool Server::isConnected()
{
	return connected;
}

int Server::sendbuf(char* buf, int buflen)
{

	cout << endl << "buf:" << buf << endl;

	if (!connected)
	{
		printf("No Connection to Client. Use connectToClient()!\n");
		return 1;
	}

	int iResult = send(ClientSocket, buf, buflen, 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		connected = false;
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);
	return 0;
}

int Server::receivebuf()
{
	if (!connected)
	{
		printf("No Connection to Client. Use connectToClient()!\n");
		return 1;
	}

	int iResult = 0;

	do
	{
		iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("No more Bytes send.\n");
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			connected = false;
			return 1;
		}
	} while (iResult > 0);

	return 0;
}

char* Server::getReceivebuf()
{
	return recvbuf;
}

int Server::shutdownConnection()
{
	int iResult = shutdown(ClientSocket, SD_SEND);

	connected = false;

	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}

