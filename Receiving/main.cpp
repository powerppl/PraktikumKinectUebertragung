#include "BufferedPrintReceiver.h"
#include "Client.h"

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