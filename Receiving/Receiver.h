#pragma once
class Receiver
{
public:
	Receiver();
	virtual ~Receiver();
	virtual int useData(char recvbuf[]) = 0;
};

