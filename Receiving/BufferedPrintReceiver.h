#pragma once
#include "Receiver.h"
#include <string>
#include <iostream>


using namespace std;

class BufferedPrintReceiver :
	public Receiver
{
public:
	BufferedPrintReceiver();
	virtual ~BufferedPrintReceiver();
	virtual int useData(char* recvbuf);

private:
	string* buffer;
};

