#include "PrintReceiver.h"


PrintReceiver::PrintReceiver()
{
}


PrintReceiver::~PrintReceiver()
{
}

int PrintReceiver::useData(char* receivebuf)
{
	std::cout << "\nGebe empfangene Daten aus:\n" << receivebuf << "\n\n";
	return 0;
}
