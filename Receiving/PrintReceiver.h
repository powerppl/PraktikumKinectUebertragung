#pragma once
#include "Receiver.h"
#include <stdio.h>
#include <iostream>

class PrintReceiver :
	public Receiver
{
public:
	PrintReceiver();
	virtual ~PrintReceiver();
	virtual int useData(char* receivebuf);
};

