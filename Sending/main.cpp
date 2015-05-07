#include "Server.h"

int main()
{
	Server* server = new Server();

	int iResult = server->connectToClient();

	if (iResult != 0) return 1;

	char input[DEFAULT_BUFLEN];

	while (true)
	{
		cout << endl << "Was soll gesendet werden? (b = beenden)" << endl;
		cin >> input;
		cout << "input:" << input << endl;


		if (strcmp(input, "b") == 0)
		{
			cout << endl << "Beendet Verbindung!" << endl;
			return server->shutdownConnection();
		}

		cout << endl << "Sendet \"" << input << "\"" << endl;

		iResult = server->sendbuf(input);

		if (iResult != 0) return iResult;
	}
}