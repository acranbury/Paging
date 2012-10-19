#ifndef RS232_H
#define RS232_H

#include <Windows.h>

#define COMPORT4 "\\\\.\\COM4"
#define COMPORT3 "\\\\.\\COM3"
#define BUFSIZE     140

int OpenRS232Port();
int WriteToRS232(BYTE * writeBuf, long *bufSize);
int SetUpDCB(int baudRate);
void ReadFromRS232(BYTE * readBuf);
void CloseRS232Port();

static HANDLE hComm;
static DCB portDCB;
static COMMTIMEOUTS timeouts;

#endif // RS232_H
