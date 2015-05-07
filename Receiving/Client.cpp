#include "Client.h"


Client::Client(Receiver* recv):
receiver(recv)
{
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
	}
}

int Client::connectToServer(char* serveraddress, char* port)
{
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult = getaddrinfo(serveraddress, port, &hints, &result);

	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	ptr = result;

	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	//eig nächste adresse prüfen

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	connected = true;
	return 0;
}

bool Client::isConnected()
{
	return connected;
}

int Client::sendbuf(char* buf, int buflen)
{
	if (!connected)
	{
		printf("No Connection to Server. Use connectToServer()!\n");
		return 1;
	}

	int iResult = send(ConnectSocket, buf, buflen, 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		connected = false;
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);
	return 0;
}

int Client::receivebuf()
{
	if (!connected)
	{
		printf("No Connection to Server. Use connectToServer()!\n");
		return 1;
	}

	int iResult=0;

	do
	{
		iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);

			receiver->useData(recvbuf);

		}
		else if (iResult == 0)
			printf("No more Bytes send.\n");
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			connected = false;
			return 1;
		}
	} while (iResult > 0);
	
	return 0;
}

char* Client::getReceivebuf()
{
	return recvbuf;
}

int Client::shutdownConnection()
{
	int iResult = shutdown(ConnectSocket, SD_SEND);

	connected = false;
	
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}


Client::~Client()
{
	if (connected)
	{
		shutdownConnection();
	}
}

int main(){
	Receiver *r = new BufferedPrintReceiver();
	Client* client = new Client(r);

	char* ipadresse = new char[500];
	cout << "Wie ist die Ip-Adresse? \n";
	cin >> ipadresse;

	int iResult = client->connectToServer(ipadresse);

	if (iResult != 0) return 1;

	char input[DEFAULT_BUFLEN];

	cout << endl << "Empfaengt Daten..." << endl;

	iResult = client->receivebuf();
	if (iResult != 0) return iResult;

	cout << endl << "Beendet Verbindung!" << endl;
	return client->shutdownConnection();
	
}
