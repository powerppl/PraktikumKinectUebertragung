#include "BufferedPrintReceiver.h"


BufferedPrintReceiver::BufferedPrintReceiver()
{
	buffer = new string();
}


BufferedPrintReceiver::~BufferedPrintReceiver()
{
	
}

int BufferedPrintReceiver::useData(char* recvbuf){
	buffer->append(recvbuf);
	if (buffer->length() > 100){
		cout << "Buffer ausgeben: " << endl << buffer->c_str() << endl;
		buffer->clear();
	}
	return 0;
}
